<?php

///////////////////////////////////////////////////////////////////////////////
// TNT-SYNERGY: PHP Application Framework                            v2.0.00 //
// ======================================                                    //
//                                                                           //
// Copyright (c) 2003 by Nick Turner                                         //
// mail:info@tnt-synergy.com                                                 //
// http://www.tnt-synergy.com/                                               //
//                                                                           //
// TNT-SYNERGY is a PHP Application Framework. It encourages application     //
// structured application architectures based on the "Model2" version of the //
// Model View Controller (MVC) design paradigm. The framework supports the   //
// following key features:                                                   //
//     - Model                                                               //
//         - session persistance (PHP Sessions)                              //
//         - permanent persistance (Database library abstraction)            //
//         - authentication (Multi-user)                                     //
//         - authorization (Access control)                                  //
//         - application model (Object based view of web page)               //
//     - Control                                                             //
//        - state processing (Page states)                                   //
//        - event processing (User supplied events)                          //
//     - View                                                                //
//        - themes (application and/or user themes)                          //
//        - template based presentation (Template library abstraction)       //
//        - internationalisation (Multi-language support)                    //
//                                                                           //
// This program is free software. You can redistribute it and/or modify it   //
// under the terms of the GNU General Public License as published by the     //
// Free Software Foundation; either version 2 of the License, or (at your    //
// option) any later version.                                                //
//                                                                           //
//   #####################################################################   //
//   # This program is distributed in the hope that it will be useful,   #   //
//   # but WITHOUT ANY WARRANTY; without even the implied warranty of    #   //
//   # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU #   //
//   # General Public License for more details.                          #   //
//   #####################################################################   //
//                                                                           //
// TNT-Synergy is a wholly owned subsidiary of TurnerTronics. The project is //
// open development and is the culmination of many peoples time and effort.  //
// For a full list of developers and copyrights please refer to CREDITS.TXT  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/**
 * Database Access Class
 * ~~~~~~~~~~~~~~~~~~~~~
 *
 * This class interfaces to the underlying database access functions, in order
 * to support multiple database types we use the MDB database abstraction
 * package.
 *
 * NB: Although the code can interface to most databases using the abstraction
 * layer all coding is tested with only MySQL at present and as such SQL
 * query statements may not work on other databases. To minimise any problems
 * all SQL queries should be coded to ANSI-92 SQL specifications only.
 *
 * @package TNT.Database
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObject');

/*
 * Globals
 */

/*
 * Defines
 */
define('TNT_DB_FETCH_NUM',      MYSQL_NUM);
define('TNT_DB_FETCH_ASSOC',    MYSQL_ASSOC);
define('TNT_DB_FETCH_BOTH',     MYSQL_BOTH);
define('TNT_DB_FETCH_STRUCT',   10);

/**
 * Database access class
 *
 * Multiple database classes can coexist, however most of the objects that
 * use the database class have the concept of a default database. This default
 * database is obtained using the standard singleton {@link Singleton()} method.
 *
 * Details for the default database object are taken from the 'Database'
 * section in the global registry, the value in this section are passed
 * directly to the object constructor.
 *
 * Unlike most other singleton classes instatiating a new object will not alter
 * the singleton object, which can only be instatiated and accessed via the
 * {@link Singleton()} method.
 *
 * @package TNT.Db
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTDatabase extends TNTObject {

    /**
     * Data Source Name (DSN) of the database
     * @var     array      $dsn     DSN name used to connect to the database
     * @access  public
     */
    var $dsn = null;

    /**
     * Prefix to put in front of psuedo table names to get real table name
     * @var     string      $prefix     default table name prefix
     * @access  public
     */
    var $prefix = '';

    /**
     * Associative array of psuedo table names to real table names
     * @var     string[]    $tables     associative array of table mappings
     * @access  public
     */
    var $tables = null;

    /**
     * Path to use for database cache files.
     * @var     string      $cachePath  filesystem path to cache directory
     * @access  public
     */
    var $cachePath = '~cache/tntdatabase/';

    /**
     * Handle to the open database connection
     * @var     resource    $_hDb       database resource handle
     * @access  private
     */
    var $_hDb= null;

    /**
     * Database table schemas
     * @var     array       $_schema    database table schemas
     * @access  private
     */
    var $_schema = null;

    /**
     * Number of rows selected, deleted, or modified by {@link Query()}
     * @var     integer     $_affectedRows number of rows affected by query
     * @access  private
     */
    var $_affectedRows = -1;

    /** 
     * Get a reference to this object - only a single instance should exist
     *
     * @access  public
     * @return  TNTSession&         reference to global TNTSession object
     * @static
     */
    /*static*/ function &Singleton() {
        $obj =& TNTObject::Singleton(__CLASS__);

        if (!$obj->IsOpen()) {
            TNT::Timestamp('Attemption to open default data source');
            // Attempt to open the default database connection
            $options = TNT::RegGetVar('database');

            if (!$options || empty($options['dsn'])) {
                TNT::Abort('TNTDatabase::Singleton() - no default data source name has been specified in the global registry');
            }

            if (!empty($options['cachePath'])) {
                $obj->cachePath = $options['cachePath'];
            }

            // Try to connect to the specified database server
            $dsn = $options['dsn'];
            unset($options['dsn']);
            if (!$obj->Connect($dsn, $options)) {
                TNT::Abort('TNTDatabase::Singleton() - failed to connect to the default data source: '.$dsn);
            }

            if (!$obj->IsOpen()) {
                TNT::Abort('TNTDatabase::Singleton() - no default database name has been specified in the global registry');
            }

            TNT::Timestamp('DSN open');
        }

        return $obj;
    }

    /** 
     * Attempt to get data from the cache.
     *
     * The database maintains its own cache, which is used to save information
     * read from the database that rarely changes.
     *
     * This method used {@link TNTFileSystem::GetCachedData()} to get the cached
     * data.
     *
     * Returns null if no cached data.
     *
     * @access  public
     * @param   string      $name       cache name
     * @return  mixed                   cached data (or null)
     */
    function GetCache($name) {

        $ret = null;

        if ($this->IsConnected()) {
            $path = "{$this->_cachePath}/{$name}.dat";
            $ret = TNTFileSystem::GetCachedData($path);
        }

        return $ret;
    }

    /** 
     * Attempt to set data in the cache.
     *
     * The database maintains its own cache, which is used to save information
     * read from the database that rarely changes.
     *
     * Set the cache to null to delete it.
     *
     * This method used {@link TNTFileSystem::SetCachedData()} to set the cached
     * data.
     *
     * Returns false if can't store or delete the cache.
     *
     * @access  public
     * @param   string      $name       cache name
     * @param   mixed       $data       data to store in cache
     * @return  boolean                 true if successfull
     */
    function SetCache($name, $data) {

        $ret = false;

        if ($this->IsConnected()) {
            $path = "{$this->_cachePath}/{$name}.dat";
            $ret = TNTFileSystem::SetCachedData($path, $data);
        }

        return $ret;
    }

    /**
     * Parse a data source name
     *
     * A array with the following keys will be returned:
     *  phptype: Database backend used in PHP (mysql, odbc etc.)
     *  dbsyntax: Database used with regards to SQL syntax etc.
     *  protocol: Communication protocol to use (tcp, unix etc.)
     *  host: Host specification
     *  port: Port number
     *  name: Database name to use
     *  user: User name for login
     *  pass: Password for login
     *
     * The format of the supplied DSN is in its fullest form:
     *
     *  phptype(dbsyntax)://user:pass@protocol+host/name
     *
     * Most variations are allowed:
     *
     *  phptype://user:pass@protocol+host:110//usr/db_file.db
     *  phptype://user:pass@host/name
     *  phptype://user:pass@host
     *  phptype://user@host
     *  phptype://host/name
     *  phptype://host
     *  phptype(dbsyntax)
     *  phptype
     *
     * @access  public
     * @param   string      $dsn            Data Source Name to be parsed
     * @return  array                       associative array
     * @static
     * @author Tomas V.V.Cox <cox@idecnet.com>
     */
    function ParseDSN($dsn) {
        
        $parsed = array(
            'phptype'  => false,
            'dbsyntax' => false,
            'user'     => false,
            'pass'     => false,
            'protocol' => false,
            'host'     => false,
            'port'     => false,
            'socket'   => false,
            'name'     => false,
        );

        if (is_array($dsn)) {
            $dsn = array_merge($parsed, $dsn);
            if (!$dsn['dbsyntax']) {
                $dsn['dbsyntax'] = $dsn['phptype'];
            }
            return $dsn;
        }

        // Find phptype and dbsyntax
        if (($pos = strpos($dsn, '://')) !== false) {
            $str = substr($dsn, 0, $pos);
            $dsn = substr($dsn, $pos + 3);

            // Get phptype and dbsyntax
            // $str => phptype(dbsyntax)
            if (preg_match('|^(.+?)\((.*?)\)$|', $str, $arr)) {
                $parsed['phptype']  = $arr[1];
                $parsed['dbsyntax'] = !$arr[2] ? $arr[1] : $arr[2];
            } else {
                $parsed['phptype']  = $str;
                $parsed['dbsyntax'] = $str;
            }
        }

        if (!strlen($dsn)) {
            return $parsed;
        }

        // Get (if found): username and password
        // $dsn => username:password@protocol+hostspec/database
        if (($at = strrpos($dsn,'@')) !== false) {
            $str = substr($dsn, 0, $at);
            $dsn = substr($dsn, $at + 1);
            if (($pos = strpos($str, ':')) !== false) {
                $parsed['user'] = rawurldecode(substr($str, 0, $pos));
                $parsed['pass'] = rawurldecode(substr($str, $pos + 1));
            } else {
                $parsed['user'] = rawurldecode($str);
            }
        }

        // Find protocol and hostspec

        // $dsn => proto(proto_opts)/database
        if (preg_match('|^([^(]+)\((.*?)\)/?(.*?)$|', $dsn, $match)) {
            $proto       = $match[1];
            $proto_opts  = $match[2] ? $match[2] : false;
            $dsn         = $match[3];

        // $dsn => protocol+hostspec/database (old format)
        } else {
            if (strpos($dsn, '+') !== false) {
                list($proto, $dsn) = explode('+', $dsn, 2);
            }
            if (strpos($dsn, '/') !== false) {
                list($proto_opts, $dsn) = explode('/', $dsn, 2);
            } else {
                $proto_opts = $dsn;
                $dsn = null;
            }
        }

        // process the different protocol options
        $parsed['protocol'] = (!empty($proto)) ? $proto : 'tcp';
        $proto_opts = rawurldecode($proto_opts);
        if ($parsed['protocol'] == 'tcp') {
            if (strpos($proto_opts, ':') !== false) {
                list($parsed['host'], $parsed['port']) = explode(':', $proto_opts);
            } else {
                $parsed['host'] = $proto_opts;
            }
        } elseif ($parsed['protocol'] == 'unix') {
            $parsed['socket'] = $proto_opts;
        }

        // Get dabase if any
        // $dsn => database
        if ($dsn) {
            // /database
            if (($pos = strpos($dsn, '?')) === false) {
                $parsed['name'] = $dsn;
            // /database?param1=value1&param2=value2
            } else {
                $parsed['name'] = substr($dsn, 0, $pos);
                $dsn = substr($dsn, $pos + 1);
                if (strpos($dsn, '&') !== false) {
                    $opts = explode('&', $dsn);
                } else { // database?param1=value1
                    $opts = array($dsn);
                }
                foreach ($opts as $opt) {
                    list($key, $value) = explode('=', $opt);
                    if (!isset($parsed[$key])) {
                        // don't allow params overwrite
                        $parsed[$key] = rawurldecode($value);
                    }
                }
            }
        }

        return $parsed;
    }

    /**
     * Connect to the database server
     *
     * Supported connection options are;
     *   + boolean 'persistant' - use a persistant connection
     *   + string 'prefix' - table name prefix to pass to {@link Open()}
     *   + string[] 'tables' - table names to pass to {@link Open()}
     *
     * The $dsn argument must be a valid Data Source Name string understood
     * by {@link TNTDatabase::ParseDSN()} or an array of the same structure
     * returned by {@link TNTDatabase::ParseDSN()}.
     *
     * @access  public
     * @param   mixed       $dsn        Data Source Name
     * @param   array       $options    construction options
     * @return  void
     */
    function Connect($dsn, $options = null) {

        $ret = null;

        // Start of error section
        do {

            if ($this->IsConnected()) {
                $this->Disconnect();
            }

            $this->_dsn = $dsn;

            if (!is_array($this->_dsn)) {
                $this->_dsn = TNTDatabase::ParseDSN($this->_dsn);
            }

            if (empty($this->_dsn['host'])) {
                $this->_dsn['host'] = 'localhost';
            }

            $dbhost = @$this->_dsn['host'];
            $dbuser = @$this->_dsn['user'];
            $dbpass = @$this->_dsn['pass'];
            $dbname = @$this->_dsn['name'];

            if (!empty($options['persistant'])) {
                $this->_hDb = @mysql_pconnect($dbhost, $dbuser, $dbpass);
            } else {
                $this->_hDb = @mysql_connect($dbhost, $dbuser, $dbpass);
            }

            if (!$this->_hDb) {
                $this->_SetError(sprintf('Can\'t connect to database server \'%s\' as \'%s/$%s\': %s', $dbhost, $dbuser, $dbpass, $this->FmtErrTxt()));
                $ret = false;
                break;
            }

            if ($dbname) {
                // Try to open the specified database on the server
                if (!$this->Open($dbname, @$options['prefix'], @$options['tables'])) {
                    $ret = false;
                    break;
                }
            }

            TNTLoader::RequireClass('TNTFileSystem');

            $this->_cachePath = TNTFileSystem::ExpandPath($this->cachePath.'/'.md5(serialize($this->_dsn)));

            TNTFileSystem::MkDir($this->_cachePath);

            $ret = true;

        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Disconnect from the database server
     *
     * @access  public
     * @return  void
     */
    function Disconnect() {

        // Start error section
        do {

            $ret = false;

            if (!$this->IsConnected()) {
                $ret = true;
                break;
            }

            $this->_cachePath = null;

            if (!mysql_close($this->_hDb)) {
                $this->_SetError(sprintf(translate('Can\'t disconnect from database server: %s'), $this->FmtErrTxt()));
                break;
            }

            $this->_hDb = null;
            $ret = true;
        
        } while (false);
        // End of error section
    
        return $ret;
    }
    
    /**
     * Are we connected to a database server
     *
     * @access  public
     * @return  boolean             true if connected to a server
     */
    function IsConnected() {
        return ($this->_hDb && is_resource($this->_hDb));
    }

    /**
     * Open a given database and configure its table mappings etc.
     *
     * @access  public
     * @param   string      $name       name of database
     * @param   string      $prefix     default table name prefix
     * @param   string      $tables     associative array of table mappings
     * @return  boolean                 true if successfull
     */
    function Open($name, $prefix = '', $tables = null) {

        $ret = false;

        // Start error section
        do {

            if (!$this->IsConnected()) {
                $this->_SetError('database is not connected');
                break;
            }

            $this->name = null;
            $this->prefix = null;
            $this->tables = null;

            if (!@mysql_select_db($name, $this->_hDb)) {
                $this->_SetError(sprintf('Can\'t open database \'%s\': %s',
                    $name, $this->FmtErrTxt()));
                break;
            }

            if ($tables && !is_array($tables)) {
                $tables = null;
            }

            $this->name = $name;
            $this->prefix = $prefix;
            $this->tables = $tables;

            $ret = true;

        } while (false);
        // End error section

        return $ret;
    }
    
    /**
     * Close a given database
     *
     * @access  public
     * @return  boolean             true if successfull
     */
    function Close() {

        $ret = false;

        // Start error section
        do {

            if (!$this->IsConnected()) {
                $this->_SetError('database is not connected');
                break;
            }

            // There is no way to tell SQL to stop using the database :-(
            $this->name = null;
            $this->prefix = null;
            $this->tables = null;

            $ret = true;

        } while (false);
        // End error section

        return $ret;
    }
    
    /**
     * Is the given database open on the server
     *
     * @access  public
     * @return  boolean             true if database is open
     */
    function IsOpen() {
        return (!empty($this->name));
    }

    /**
     * Return the real name of a table given its psuedo name.
     *
     * Tables are refered to by an psuedo name - this function allows use
     * to map the psuedo name to the real table name
     *
     * This is to allow tables to be renamed without changing all of the code
     * if a site only allows one database and our table names clash with
     * existing table names.
     *
     * NB: Fields are not psuedoed
     *
     * @access  public
     * @param   string      $name       psuedo table name
     * @return  string                  the real database table name
     */
    function TableName($name) {
        return (isset($this->tables[$name]) ? $this->tables[$name] : $this->prefix.$name);
    }
    
    /**
     * Escape a string that is to be used in an SQL query.
     *
     * Numeric strings are not quoted !
     *
     * All reserved characters in the string will be escaped.
     *
     * Non scalar values will either abort the code (if $abort is true)
     * or return the 'NULL' string (NB: not the NULL value).
     *
     * @access  public
     * @param   scalar      $string     the stringto escape
     * @param   boolean     $abort      abort if non scalar value
     * @return  scalar                  the escaped string
     */
    function EscapeString($string, $abort = true) {

        static $has_real = -1;
        static $has_fake = -1;

        // We test for the functions exists once only, for efficiency
        if ($has_real == -1) {
           $has_real = function_exists('mysql_real_escape_string');
        }
        if ($has_fake == -1) {
           $has_fake = function_exists('mysql_escape_string');
        }

        if (!is_numeric($string)) {
            if (!is_scalar($string)) {
                if ($abort) {
                    trigger_error('TNTDatabase::EscapeString() - can not escape a non scalar value: '.var_info($string));
                    die();
                } else {
                    return 'NULL';
                }
            }
            if ($has_real && $this->IsConnected()) {
                $string = mysql_real_escape_string($string, $this->_hDb);
            } else if ($has_fake) {
                $string = mysql_escape_string($string);
            } else {
                $string = addslashes($string);
            }
        }
    
        return $string;
    }
    
    /**
     * Quote a string that is to be used in an SQL query.
     *
     * All reserved characters in the string will be escaped and then the
     * string will be quoted.
     *
     * This calls {@link EscapeString()} and then returns the resultant
     * escaped string within quotes.
     *
     * Non scalar values will either abort the code (if $abort is true)
     * or return the 'NULL' string (NB: not the NULL value).
     *
     * @access  public
     * @param   scalar      $string     the string to quote
     * @param   boolean     $abort      abort if non scalar value
     * @return  string                  the escaped string
     */
    function QuoteString($string, $abort = true) {
    
        $ret = $this->EscapeString($string, $abort);

        return "'{$ret}'";
    }
    
    /**
     * Prepare a value for use in an SQL query.
     *
     * If the value is not a scalar and $csf is true then {@link ContractCSF()}
     * is called the resultant value is passed through {@link QuoteString()} and
     * returned. If $csf is false then the code either aborts if $abort is true
     * or returns the string 'NULL'.
     *
     * If the value is a string then it passed through {@link QuoteString()}
     * and return.
     *
     * Other values are just cast to strings and returned.
     *
     * @access  public
     * @param   mixed       $value      the value to prepare
     * @param   boolean     $csf        automatically expand compound sub fields
     * @param   boolean     $abort      abort if non scalar value
     * @return  string                  the escaped string
     */
    function PrepareVal($value, $csf = true, $abort = true) {
    
        if (is_null($value)) {
            return 'NULL';
        }

        if (!is_scalar($value)) {
            if ($csf) {
                return $this->QuoteString($this->ContractCSF($value));
            } else if ($abort) {
                TNT::Abort('TNTDatabase::PrepareVal() - can\'t prepare compound data');
                return null;
            } else {
                return 'NULL';
            }
        }

        if (is_string($value)) {
            return $this->QuoteString($value);
        }

        return (string)$value;
    }
    
    /**
     * Perform a database query
     *
     * The database query is expanded by replacing %xxx% psuedo
     * table/field names with real table/field names.
     *
     * @access  public
     * @param   string      $sql        the sql to execute
     * @return  mixed                   boolean or TNTDbResult object (or null)
     */
    function Query($sql) {

        $ret = null;

        $this->_affectedRows = -1;

        // Start error section
        do {
            if (!$this->IsOpen()) {
                $this->_SetError('database is not open');
                break;
            }

            $result = null;

            // Replace all database table references - allows you to use the
            // psuedo table names in the queries.
            $sql = $this->ExpandSql($sql);
    
            $result = @mysql_query($sql, $this->_hDb);
    
            if (!$result) {
                $this->_SetError(sprintf('Can\'t perform query \'%s\': %s', (strlen($sql) < 256 ? $sql : substr($sql, 0, 256).'...'), $this->FmtErrTxt()));
                break;
            }

            if (is_bool($result)) {
                $this->_affectedRows = mysql_affected_rows($this->_hDb);
                $ret = $result;
            } else if (is_resource($result)) {
                $this->_affectedRows = mysql_num_rows($result);
                TNTLoader::RequireClass('TNTDbResult');
                $ret =& new TNTDbResult($result, $sql, &$this);
            } else {
                $this->_SetError('Unknown return type from mysql_query(): '.gettype($result));
                break;
            }

        } while (false);
        // End error section
    
        return $ret;
    }

    /**
     * Fetch all the rows that match the given query.
     *
     * This is an optimised call as it doesn't create a TNTDbResult object
     * to retrieve the results.
     *
     * The method returns null on error or if the SQL statement was not a
     * "SELECT" statement. If no rows matched the SQL statement then an
     * empty array is returned.
     *
     * The number if rows returned can be limited by setting the $max argument
     * (a zero value means all rows). The actual number of rows selected by
     * the method can be obtained my calling {@link NumAffectedRows()}.
     *
     * The $idx argument can be used to name a field whos value will be used
     * to index thats rows entry in the returned array. This must be a unique
     * key field or row overwriting may occur. If no $idx is given then the
     * rows are indexed numerically sequentially starting from zero.
     *
     * @access  public
     * @param   string      $sql        the sql to execute
     * @param   integer     $max        the maximum number of rows to return
     * @param   integer     $type       return rows numeric, associative or both
     * @param   boolean     $csf        automatically expand compound sub fields
     * @param   scalar      $idx        index returned row by this field
     * @return  array                   array of database rows
     */
    function Fetch($sql, $max = 0, $type = TNT_DB_FETCH_ASSOC, $csf = false, $idx = null) {

        $ret = null;

        $this->_affectedRows = -1;

        // Start error section
        do {
            if (!$this->IsOpen()) {
                $this->_SetError('database is not open');
                break;
            }

            $result = null;

            // Replace all database table references - allows you to use the
            // psuedo table names in the queries.
            $sql = $this->ExpandSql($sql);
    
            $result = @mysql_query($sql, $this->_hDb);
    
            if (!$result) {
                $this->_SetError(sprintf('Can\'t perform query \'%s\': %s', (strlen($sql) < 256 ? $sql : substr($sql, 0, 256).'...'), $this->FmtErrTxt()));
                break;
            }

            if (is_bool($result)) {
                $this->_affectedRows = mysql_affected_rows($this->_hDb);
                $ret = array();
            } else if (is_resource($result)) {
                // Fetch all the rows
                $ret = array();
                $this->_affectedRows = mysql_num_rows($result);

                $struct = null;
                if ($type == TNT_DB_FETCH_STRUCT) {
                    // Prefix the fetched field with its table name
                    $flds = array();
                    $max_i = mysql_num_fields($result);
                    for ($i = 0; $i < $max_i; $i++) {
                        $struct[$i] = array('table' => mysql_field_table($result, $i), 'field' => mysql_field_name($result, $i));
                    }
                    $type = TNT_DB_FETCH_NUM;
                }

                $r = 0;
                while ($row = mysql_fetch_array($result, $type)) {
                    if (!$idx) {
                        $_r = $r;
                    } else if (isset($row[$idx])) {
                        $_r = $row[$idx];
                    } else {
                        $this->_SetError("Index field '$idx' is not present in row");
                        $ret = null;
                        break;
                    }

                    if ($csf || $struct) {
                        $_row = array();
                        foreach ($row as $n => $v) {
                            if ($csf && $v && $v{0} == 'c') {
                                $v = $this->ExpandCSF($v, $n);
                            }
                            if ($struct) {
                                $_s = $struct[$n];
                                $_row[$_s['table']][$_s['field']] = $v;
                            } else {
                                $_row[$n] = $v;
                            }
                        }
                        $row = $_row;
                    }
                    if (is_null($idx)) {
                       $ret[$_r] = $row;
                    } else {
                        $this->_SetError("Index field '$idx' is not present in row");
                        $ret = null;
                        break;
                    }

                    ++$r;

                    if ($max && $r >= $max) {
                        break;
                    }
                }
            } else {
                $this->_SetError('Unknown return type from mysql_query(): '.gettype($result));
                break;
            }

        } while (false);
        // End error section

        return $ret;
    }
    
    /**
     * Fetch a single row and single field from the database.
     *
     * This is shortcut call to {@link Fetch()} where a single value is
     * required. The method returns the first field from the first row
     * returned by the SQL statement.
     *
     * @access  public
     * @param   string      $sql        the sql to execute
     * @param   boolean     $csf        automatically expand compound sub fields
     * @return  mixed                   value of first field of first row
     */
    function FetchValue($sql, $csf = false) {
        $rows = $this->Fetch($sql, 1, TNT_DB_FETCH_NUM, $csf);
        return (isset($rows[0][0]) ? $rows[0][0] : null);
    }

    /**
     * Split a complex query into an array of individual queries
     *
     * [This function was derived from the function in the file
     *  'read_dump.php' released as part of the phpMyAdmin project.
     *  Copyright (c) 1998-2000 Tobias Ratschiller]
     *
     * @access  public
     * @param   string      $sql        the cpmlex sql to split
     * @return  array                   array of single statement queries
     */
    function SplitQueries($sql) {

        $sql          = trim($sql);
        $sql_len      = strlen($sql);
        $char         = '';
        $string_start = '';
        $in_string    = false;
        $time0        = time();

        for ($i = 0; $i < $sql_len; ++$i) {
            $char = $sql[$i];
    
            // We are in a string, check for not escaped end of strings except
            // for backquotes that can't be escaped
            if ($in_string) {
                for (;;) {
                    $i = strpos($sql, $string_start, $i);
                    // No end of string found -> add the current substring to
                    // the returned array
                    if (!$i) {
                        $ret[] = $sql;
                        return $ret;
                    }
                    // Backquotes or no backslashes before quotes: it's indeed
                    // the end of the string -> exit the loop
                    else if ($string_start == '`' || $sql[$i-1] != '\\') {
                        $string_start      = '';
                        $in_string         = FALSE;
                        break;
                    }
                    // one or more Backslashes before the presumed end of
                    // string...
                    else {
                        // ... first checks for escaped backslashes
                        $j                     = 2;
                        $escaped_backslash     = FALSE;
                        while ($i-$j > 0 && $sql[$i-$j] == '\\') {
                            $escaped_backslash = !$escaped_backslash;
                            $j++;
                        }
                        // ... if escaped backslashes: it's really the end of
                        // the string -> exit the loop
                        if ($escaped_backslash) {
                            $string_start  = '';
                            $in_string     = FALSE;
                            break;
                        }
                        // ... else loop
                        else {
                            $i++;
                        }
                    } // end if...elseif...else
                } // end for
            } // end if (in string)
    
            // We are not in a string or comment, first check for delimiter...
            else if ($char == ';') {
                // if delimiter found, add the parsed part to the returned array
                $ret[]      = substr($sql, 0, $i);
                $sql        = ltrim(substr($sql, min($i + 1, $sql_len)));
                $sql_len    = strlen($sql);
                if ($sql_len) {
                    $i      = -1;
                } else {
                    // The submited statement(s) end(s) here
                    return $ret;
                }
            } // end else if (is delimiter)
    
            // ... then check for start of a string,...
            else if (($char == '"') || ($char == '\'') || ($char == '`')) {
                $in_string    = true;
                $string_start = $char;
            } // end else if (is start of string)
    
            // ... for start of a comment (and remove this comment if found)...
            else if ($char == '#'
                     || ($char == ' ' && $i > 1 && $sql[$i-2] . $sql[$i-1] == '--')) {
                
                // starting position of the comment depends on the comment type
                $start_of_comment = (($sql[$i] == '#') ? $i : $i-2);
                // if no "\n" exits in the remaining string, checks for "\r"
                // (Mac eol style)
                $end_of_comment   = (strpos(' ' . $sql, "\012", $i+2))
                                  ? strpos(' ' . $sql, "\012", $i+2)
                                  : strpos(' ' . $sql, "\015", $i+2);
                if (!$end_of_comment) {
                    // no eol found after '#', add the parsed part to the
                    // returned array if required and exit
                    if ($start_of_comment > 0) {
                        $ret[]    = trim(substr($sql, 0, $start_of_comment));
                    }
                    return $ret;
                } else {
                    $sql          = substr($sql, 0, $start_of_comment)
                                  . ltrim(substr($sql, $end_of_comment));
                    $sql_len      = strlen($sql);
                    $i--;
                } // end if...else
            } // end else if (is comment)
    
          // ... and finally disactivate the "/*!...*/" syntax if
          // MySQL < 3.22.07
    /*
          else if ($release < 32270
                   && ($char == '!' && $i > 1  && $sql[$i-2] . $sql[$i-1] == '/*')) {
              $sql[$i] = ' ';
          } // end else if
    */
    
    
            // loic1: send a fake header each 30 sec. to bypass browser timeout
            $time1     = time();
            if ($time1 >= $time0 + 30) {
                $time0 = $time1;
                header('X-pmaPing: Pong');
            } // end if
        } // end for
    
        // add any rest to the returned array
        if (!empty($sql) && ereg('[^[:space:]]+', $sql)) {
            $ret[] = $sql;
        }
    
        return $ret;
    }
    
    /**
     * Expand a token
     *
     *  Tokens are of the form %[<db>.]<table>[.<field>]%
     *  If 1 element is present then it is assumed to be <table>
     *  If 2 elements are present then it is assumed to be <table>.<field>
     *
     * @access  private
     * @param   array       $matches    regex matches
     * @return  string                  resultant SQL to replace token
     */
    function _ExpandSql($matches) {
        if (!$matches || !is_array($matches) || count($matches) != 2) {
            return null;
        }
    
        $s = explode('.', $matches[1]);
        switch (count($s)) {
            case 3:
                $s[1] = $this->TableName($s[1]);
                break;
            case 2:
                $s[0] = $this->TableName($s[0]);
                break;
            case 1:
                $s[0] = $this->TableName($s[0]);
                break;
        }
        return implode('.', $s);
    }

    /**
     * Expand a SQL statement replace all psuedo tokens %xxx% with their
     * correct values.
     *
     *  Tokens are of the form " %[<db>.]<table>[.<field>]% "
     *  If 1 element is present then it is assumed to be <table>
     *  If 2 elements are present then it is assumed to be <table>.<field>
     *
     * Tokens must be surrounded by whitespace or commas and shouldn't be
     * replaced if in quotes (but we will deal with that later)
     *
     * @access  public
     * @param   string      $sql        the sql to expand
     * @return  string                  expanded sql
     */
    function ExpandSql($sql) {
        if (strstr($sql, '%')) {
            $sql = preg_replace_callback('/(?<=[\s,])%([^%]+)%(?=[\s,]|$)/m',
                array($this, '_ExpandSql'), $sql);
        }
        return $sql;
    }
    
    /**
     * Prepare query.
     *
     * Allows you to format a complex query in a printf type format, where
     * the data is automatically quoted, converted as required.
     *
     * @access  private
     * @param   string      $query      the query to prepare.
     * @param   array       $data       the data to add to the query
     * @param   
     * @return  string                  the prepared query
     */
    function _PrepareSql($matches) {
        if (!$matches || !is_array($matches) || count($matches) != 3) {
            return 'NULL';
        }

        $name = $matches[2];

        if (!isset($this->_prepareSqlData[$name])) {
            return 'NULL';
        }

        return $this->PrepareVal($this->_prepareSqlData[$name]);
    }

    /**
     * Prepare a SQL statement replacing all data tokens with the corresponding
     * data converted, quoted etc. as required.
     *
     * Data tokens are of the form "#name", where name is the field name in
     * the data array supplied.
     *
     * If a data field is referenced which is not in the data array then a
     * null value is used.
     *
     * For example;
     *   "SELECT * FROM %users% WHERE (id=#userid || name=#name || age=#age)"
     * prepared with the data;
     *   array('id' => 22, 'name' => 'fred');
     * will be expanded to;
     *   "SELECT * FROM %users% WHERE (id=22 || name='fred' || age=NULL)"
     *
     * String data fields are automatically quoted as required, and inner
     * quotes are escaped. Arrays/Objects are added as compound fields.
     *
     * NB: Table, field references are not expanded during the preparation.
     *
     * @access  public
     * @param   string      $sql        the sql to expand
     * @param   array       $data       the data
     * @return  string                  expanded sql
     */
    function PrepareSql($sql, $data) {
        if (strstr($sql, '#')) {
            $this->_prepareSqlData =& $data;
            $sql = preg_replace_callback('/(#(\w*))/m',
                array($this, '_PrepareSql'), $sql);
        }
        return $sql;
    }
    
    /**
     * Perform a series of database queries parsed from a string
     *
     * @access  public
     * @param   string      $sql        the complex sql to parse
     * @return  handle                  result of last statement (or null)
     */
    function Queries($sql) {
    
        // Replace all database table references - allows you to use the psuedo
        // table names in the queries.
        $sql = $this->ExpandSql($sql);
    
        // Split the string into an array of individual queries
        if (!($sql_array = $this->SplitQueries($sql))) {
            $ret = false;
        } else {
            $ret = true;
            $sql_count = count($sql_array);
            for ($i = 0; $i < $sql_count; $i++) {
                if (!($ret = $this->Query($sql_array[$i]))) {
                    break;
                }
                destroy($ret);
            }
            unset($sql_array);
        }
    
        return $ret;
    }
    
    /**
     * Perform a series of database queries parsed from a file
     *
     * @access  public
     * @param   string      $path       the path to the file to read
     * @param   array       $vars       variables to use for template
     * @return  handle                  result of last statement (or null)
     */
    function QueriesFile($path, $vars = null) {

        $file = &new TNTFile($path, TNT_FILE_BINARY);

        if (($data = $file->GetContents())) {
            if ($vars) {
                // Replace all template references if required
                $sql = TNTConvert::Template($sql, $vars);
            }

            $ret = $this->Queries($sql);

        } else {
            $this->_SetError('Can\'t get contents of file: '.$file);
            $ret = false;

        }
    
        return $ret;
    }
    
    /**
     * Return the last error text from the database server (or null if no error)
     *
     * @access  public
     * @return  string              the description of the last error
     */
    function ErrorText() {
        $error = ($this->IsConnected() ? mysql_error($this->_hDb) : null);
        return (!empty($error) ? $error : null);
    }
    
    /**
     * Return the last error code from the database server (or 0 if no error)
     *
     * @access  public
     * @return  integer             the numeric code of the last error
     */
    function ErrorCode() {
        $errno = ($this->IsConnected() ? mysql_errno($this->_hDb) : 0);
        return ($errno);
    }
    
    /**
     * Generate a nicely formated database error message
     *
     * @access  public
     * @param   integer     $errnum     error number (or 0 for last error)
     * @param   string      $errmsg     error message (or null for last message)
     * @return  string                  formatted string for error
     */
    function FmtErrTxt($errnum = 0, $errmsg = null) {
        if (!$errnum) {
            $errnum = $this->ErrorCode();
        }
        if (!$errmsg) {
            $errmsg = $this->ErrorText();
        }
    
        $text = 'Db Error';
        if (function_exists('translate')) {
            $text = translate($text);
        }
    
        $text .= ": $errnum - $errmsg";
    
        return $text;
    }
    
    /**
     * Generate the SQL for a case statement to select from an array of options
     *
     * @access  public
     * @param   string      $name       field name
     * @param   array       $opts       associative array of value => text
     * @return  string                  the SQL to use in the SELECT statement
     */
    function FmtSqlCaseCmd($name, $opts) {
        $sql = "CASE $name ";
        reset($opts);
        while (list($idx, $val) = each($opts)) {
            $sql .= "WHEN '".$this->EscapeString($idx)."' THEN '".$this->EscapeString($val)."' ";
        }
        $sql .= "ELSE $name END";
        return $sql;
    }
    
    /**
     * Expand a compound sub field
     *
     * Compound sub fields (CSF) allow a field to store a non scalar value, such
     * as an associative array, etc.
     *
     * If the data is not part of a compound sub field it is return unaltered.
     *
     * @access  public
     * @param   string      $data       field data
     * @param   string      $name       field name
     * @return  array                   expanded data
     */
    function ExpandCSF($data, $name) {
        $ret = null;
    
        if (empty($data) || strncasecmp($data, 'csf:', 4) != 0) {
            return $data;
        }
    
        // A subfield can be either in PHP serialize of URL query format
        if ($data[4] == 'z') {
            $ret = @unserialize(substr($data, 6));
        } else if ($data[4] == 'q') {
            $ret = @urlquery_decode(substr($data, 6));
        } else {
            TNT::Abort('TNTDatabase::ExpandCSF() - unsupported CSF format: '.$data[4]);
        }
    
        if (!$ret) {
            $this->_SetError('Can\'t expand sub field: '.$name);
            $ret = null;
        }
    
        return $ret;
    }
    
    /**
     * Contract a compound sub field
     *
     * Compound sub fields (CSF) allow a field to store a non scalar value, such
     * as an associative array, etc.
     *
     * @access  public
     * @param   string      $name       field name
     * @param   array       $data       expanded data
     * @param   boolean     $serialize  use serialization 
     * @return  string                  compounded data string
     */
    function ContractCSF($data, $serialize = true) {
        $ret = null;
    
        if (!$data) {
            return null;
        }
        if (!is_array($data) && !is_object($data)) {
            return $data;
        }
    
        // A subfield can be either in PHP serialize of URL query format
        if ($serialize) {
            $ret = 'csf:z:'.serialize($data);
        } else {
            $ret = 'csf:q:'.urlquery_encode($data);
        }

        return $ret;
    }
    
    /**
     * Return the number of rows affected by the last call to
     * {@link Query()} or {@link Fetch()}.
     *
     * Returns the number of rows affected by the last INSERT, UPDATE or
     * DELETE query or the number of rows selected by the last SELECT query.
     *
     * If the last query failed then this will be -1.
     *
     * @access  public
     * @return  integer             number of rows affected by previous query
     */
    function NumAffectedRows() {
        return $this->_affectedRows;
    }
    
    /**
     * Return the id of the last inserted item
     *
     * @access  public
     * @return  integer             id of the last inserted item
     */
    function InsertId() {
        return mysql_insert_id($this->_hDb);
    }

    /**
     * Return the database scheme for a given table.
     *
     * Returned schema is an array of structure containing the
     * following fields;
     *   'name'     - field name
     *   'type'     - normalized type (see below)
     *   'phptype'  - appropriate php type (ie. 'number', 'string')
     *   'sqltype'  - raw SQL type (ie. 'VARCHAR(32)')
     *   'length'   - field length (or display length)
     *   'key'      - type of key 'primary', 'secondary'
     *   'default'  - default value
     *   'null'     - true is null values are allowed
     *   'extra'    - extra info returned from implimentation
     *
     * Schemas are cached to disc, so external changes to the database schema
     * could invalidate the cache.
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get schema for
     * @return  array[]                 associative array representing schema
     */
    function Schema($table) {

        $ret = null;

        // Start error section
        do {
            if (!$this->IsConnected()) {
                $this->_SetError('no database connection');
                break;
            }

            $cacheName = "schema";

            if (!isset($this->_schema)) {
                $this->_schema = $this->GetCache($cacheName);
            }

            if (isset($this->_schema[$table])) {
                $ret = $this->_schema[$table];
                break;
            }

            $sql = 'DESC '.$this->TableName($table);
            if (!($rows = $this->Fetch($sql, 0, TNT_DB_FETCH_ASSOC, false))) {
                $this->_SetError('can\'t read table schema from database');
                break;
            }

            $ret = array();
            foreach ($rows as $row) {

                $row = array_change_key_case($row, CASE_LOWER);

                $row['name'] = $row['field'];
                unset($row['field']);

                // normalize the data type
                $row['sqltype'] = $row['type'];

                if (!preg_match('/^(\w+)(?:\((\d+)\))?(.*)$/', $row['sqltype'], $matches))
                {
                    $this->_SetError('can\'t decode sql type: '.$row['sqltype']);
                    $ret = null;
                    break;
                }


                switch ($matches[1]) {
                    case 'int':
                    case 'tinyint':
                    case 'smallint':
                    case 'mediumint':
                    case 'bigint':
                        $row['type'] = 'integer';
                        $row['phptype'] = 'integer';
                        break;
                    case 'float':
                    case 'double':
                        $row['type'] = 'float';
                        $row['phptype'] = 'float';
                        break;
                    case 'varchar':
                        $row['type'] = 'text';
                        $row['phptype'] = 'string';
                        break;
                    case 'date':
                        $row['type'] = 'date';
                        $row['phptype'] = 'string';
                        break;
                    case 'time':
                        $row['type'] = 'time';
                        $row['phptype'] = 'string';
                        break;
                    case 'datetime':
                        $row['type'] = 'datetime';
                        $row['phptype'] = 'string';
                        break;
                    case 'timestamp':
                        $row['type'] = 'timestamp';
                        $row['phptype'] = 'string';
                        break;
                    case 'blob':
                    case 'tinyblob':
                    case 'mediumblob':
                    case 'longblob':
                        $row['type'] = 'blob';
                        $row['phptype'] = 'string';
                        break;
                    case 'text':
                    case 'tinytext':
                    case 'mediumtext':
                    case 'longtext':
                        $row['type'] = 'clob';
                        $row['phptype'] = 'string';
                        break;
                    case 'enum':
                        $row['type'] = 'integer';
                        $row['phptype'] = 'integer';
                        break;
                    case 'set':
                        $row['type'] = 'text';
                        $row['phptype'] = 'string';
                        break;
                    default:
                        $this->_SetError('MYSQL data type is unknown: '.$matches[1]);
                        $ret = null;
                        break;
                }
                if (is_null($ret)) {
                    break;
                }

                $row['length'] = null;
                if (!empty($matches[2])) {
                    if (!is_numeric($matches[2])) {
                        $this->_SetError('MYSQL data length is not numeric: '.$matches[2]);
                        $ret = null;
                        break;
                    }
                    $row['length'] = (int)$matches[2];
                }

                $row['extra'] .= $matches[3];

                switch ($row['key']) {
                    case '':
                        $row['key'] = '';
                        break;
                    case 'PRI':
                        $row['key'] = 'primary';
                        break;
                    case 'MUL':
                        $row['key'] = 'secondary';
                        break;
                    default:
                        break;
                }

                $ret[$row['name']] = $row;
            }

            if (!empty($ret)) {
                $this->_schema[$table] = $ret;
                $this->SetCache($cacheName, $this->_schema);
            }

        } while (false);
        // End error section

        return $ret;
    }
    
    /**
     * Return if a table exists in the schema.
     *
     * Results of this function are cached, so external changes to the
     * database schema could invalidate the cache.
     *
     * @access  public
     * @param   string      $table      psuedo name of table to check
     * @return  boolean                 true if table exists
     */
    function TableExists($table) {

        $ret = null;

        // Start error section
        do {
            if (!$this->IsConnected()) {
                $this->_SetError('no database connection');
                break;
            }

            $ret = ($this->Schema($table) != null);

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Return schema details for tables primary key. This is the key that
     * we use to index a record. It is usually a unique key.
     *
     * Results of this function are cached, so external changes to the
     * database schema could invalidate the cache.
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get fields for
     * @return  array                   array containing primary key details
     */
    function TableIndex($table) {

        $ret = null;

        // Start error section
        do {
            if (isset($this->_primary[$table])) {
                break;
            }

            if (!$this->IsConnected()) {
                $this->_SetError('no database connection');
                break;
            }

            // Set the value so isset() doesn't fail (can't use null)
            $this->_primary[$table] = false;

            if (!($schema = $this->Schema($table))) {
                break;
            }

            foreach ($schema as $field) {
                if ($field['key'] == 'primary') {
                    $this->_primary[$table] = $field;
                    break;
                }
            }
        } while (false);
        // End error section

        $ret = ($this->_primary[$table] !== false ? $this->_primary[$table] : null);

        return $ret;
    }
    
    /**
     * Return a list of field names for a table
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get fields for
     * @param   string      $sep        separator to use between field names
     * @param   boolean     $full       return full <table>.<field> format names
     * @param   boolean     $text       return text fields only
     * @return  string                  list of field names
     */
    function TableFieldNames($table, $sep = ',', $full = false, $text = false) {

        $ret = null;


        if (($schema = $this->Schema($table))) {

            $dbtable = $this->TableName($table);

            foreach ($schema as $field) {
                if ($text && $field['type'] != 'text') {
                    // Only return text based fields for text based queries etc.
                    continue;
                }
                if ($full) {
                    $names[] = $dbtable.'.'.$field['name'];
                } else {
                    $names[] = $field['name'];
                }
            }

            $ret = ($names ? join($sep, $names) : null);
        }

        return $ret;
    }

    /**
     * Return if a field exists in the table.
     *
     * It retrieves the table schema and checks that it contains the given
     * field. If $type is specified then the field must be of that type.
     *
     * Results of this function are cached, so external changes to the
     * database schema could invalidate the cache.
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get fields for
     * @param   string      $field      psuedo field name
     * @param   string      $type       field type
     * @return  boolean                 true if table has the given field
     */
    function TableFieldExists($table, $field, $type = null) {

        $ret = null;

        // Start error section
        do {
            if (isset($this->_fieldExists[$table.$field.$type])) {
                break;
            }

            if (!$this->IsConnected()) {
                $this->_SetError('no database connection');
                break;
            }

            // Set the value so isset() doesn't fail (can't use null)
            $this->_fieldExists[$table.$field.$type] = false;

            if (!($schema = $this->Schema($table))) {
                break;
            }

            // Search the schema for a field with the correct name
            if (!isset($schema[$field])) {
                // Field not in table
                break;
            }

            if (!is_null($type) && $schema[$field]['type'] != $type) {
                // Field not of correct type
                break;
            }

            $this->_fieldExists[$table.$field.$type] = true;

        } while (false);
        // End error section

        $ret = !empty($this->_fieldExists[$table.$field.$type]);

        return $ret;
    }
    
    /**
     * Return if a given table is timestamped.
     *
     * The table is assumed to be timestamped if it contains a 'mtime' field
     * of type 'integer'. If this field exists then the fields 'ctime' and
     * 'atime' are assumed to also exist and failure for these to exist will
     * cause database errors when the record handling functions are called.
     *
     * When a record handling function is used the timestamps are automatically
     * updated, any timestamp field specified in the record will not be used
     * to update the timestamps. When a record is read the timestamp fields
     * will be returned in both UNIX/PHP and native format. The UNIX/PHP
     * timestamp fields will be called '_atime', '_mtime', '_ctime'.
     *
     * This does not support the MySQL 'timestamp' field type as this is
     * formatted differently to the UNIX/PHP timestamp we use and instead of
     * perform conversion each time we just use an integer field to store the
     * UNIX/PHP timestamp. If a MySQL timestamp is required then the
     * 'FROM_UNIXTIME()' SQL function can be used.
     *
     * Results of this function are cached, so external changes to the
     * database schema could invalidate the cache.
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get fields for
     * @return  boolean                 true if table has a timestamp
     */
    function TableHasTimestamp($table) {

        $ret = $this->TableFieldExists($table, 'mtime', 'integer');

        return $ret;
    }
    
    /**
     * Count how many records match a given 'WHERE' clause
     *
     * If where clause is not provided then it counts how many records in the
     * table.
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get fields for
     * @param   string      $where      SQL where clause
     * @return  integer                 number of matching rows
     */
    function CountWhere($table, $where = null) {
        $dbtable = $this->TableName($table);
        $sql = "SELECT COUNT(*) FROM $dbtable";
        if ($where && $where != '') {
            $sql .= " WHERE $where";
        }
        return $this->FetchValue($sql, false);
    }
    
    /**
     * Count how many records have a field with a given value
     *
     * If field and value are not provided then it counts how many records in
     * the table.
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get fields for
     * @param   string      $fld        field name (or null)
     * @param   scalar      $val        field value (or null)
     * @return  integer                 number of matching rows
     */
    function Count($table, $fld=null, $val=null) {
    
        $where = null;
        if ($fld) {
            if (!is_null($val)) {
                $where = $fld.'='.$this->PrepareVal($val);
            } else {
                $where = "$fld NOT NULL";
            }
        } 
    
        return $this->CountWhere($table, $where);
    }
    
    /**
     * Load a record on a given key
     *
     * Designed to load records on a unique primary key - where only one record
     * will exist.
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get fields for
     * @param   string      $key        psuedo name of key field
     * @param   scalar      $keyval     value of key field to load record on
     * @param   string      $flds       fields to return
     * @param   integer     $type       return numeric, assoc. or both array
     * @param   boolean     $csf        automatically expand compound sub fields
     * @return  array                   array of field values
     */
    function LoadArray($table, $key, $keyval, $flds = '*', $type = TNT_DB_FETCH_ASSOC, $csf = false) {

        $ret = null;

        $keyval = $this->EscapeString($keyval);
        $dbtable = $this->TableName($table);

        $rows = $this->Fetch("SELECT $flds FROM $dbtable WHERE $key='$keyval'", 1, $type, $csf);

        if (isset($rows[0])) {
            $ret = $rows[0];
        }

        return $ret;
    }
    
    /**
     * Load a record on a given key
     *
     * Designed to load records on a unique primary key - where only one record
     * will exist.
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get fields for
     * @param   string      $key        psuedo name of key field
     * @param   scalar      $keyval     value of key field to load record on
     * @param   string      $flds       fields to return
     * @param   boolean     $csf        automatically expand compound sub fields
     * @return  array                   numeric array of field values
     */
    function LoadRow($table, $key, $keyval, $flds = '*', $csf = false) {

        $ret = null;

        $keyval = $this->EscapeString($keyval);
        $dbtable = $this->TableName($table);

        $rows = $this->Fetch("SELECT $flds FROM $dbtable WHERE $key='$keyval'", 1, TNT_DB_FETCH_NUM, $csf);

        if (isset($rows[0])) {
            $ret = $rows[0];
        }

        return $ret;
    }
    
    /**
     * Load a record on a given key
     *
     * Designed to load records on a unique primary key - where only one record
     * will exist.
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get fields for
     * @param   string      $key        psuedo name of key field
     * @param   scalar      $keyval     value of key field to load record on
     * @param   string      $flds       fields to return
     * @param   boolean     $csf        automatically expand compound sub fields
     * @return  object                  object with members for each field
     */
    function LoadObject($table, $key, $keyval, $flds = '*', $csf = false) {

        $ret = null;

        $keyval = $this->EscapeString($keyval);
        $dbtable = $this->TableName($table);

        $rows = $this->Fetch("SELECT $flds FROM $dbtable WHERE $key='$keyval'", 1, TNT_DB_FETCH_ASSOC, $csf);

        if (isset($rows[0])) {
            $ret = (object)$rows[0];
        }

        return $ret;
    }
    
    /**
     * Update an access time stamp for this record
     * (only call if record not accessed via one of the $this->XXXXRec()
     * functions)
     *
     * @access  public
     * @param   string      $table      psuedo name of table to get fields for
     * @param   string      $key        psuedo name of key field
     * @param   scalar      $val        value of key field to load record on
     * @param   string      $stamp      name of timestamp field to update
     * @return  boolean                 true if timestamp updated successfully
     */
    function UpdateTimeStamp($table, $key, $val, $stamp = 'atime') {
    
        assert('$table && $key && $val');

        $ret = false;

        if ($this->TableFieldExists($table, $stamp, 'timestamp')) {
            // We update the time stamp in the database to the current time
            $tab = $this->TableName($table);
            $time = time();
            $sql = "UPDATE $tab SET $stamp='$time' WHERE $key='$val'";
            if (!$this->Query($sql)) {
                trigger_error('TNTDatabase::UpdateTimeStamp() - can not update record timestamp: '.$this->db->Error(), E_USER_NOTICE);
            }
        }
    
        return $ret;
    }

    /**
     * Make a SET clause from the given data.
     *
     * @access  public
     * @param   array       $set        associative array of field => value
     * @param   boolean     $csf        automatically expand compound sub fields
     * @return  string                  SQL SET clause
     */
    function PrepareSet($set, $csf = true) {
    
        $ret = false;
    
        if (!$set) {
            TNT::Abort('TNTDatabase::PrepareSet() - invalid arguments');
        }
    
        // Fields to set are stored here
        $sets = null;
        foreach ($set as $fld => $val) {
            $sets[] = $fld.'='.$this->PrepareVal($val, $csf);
        }

        $ret = ($sets ? 'SET '.join(',', $sets) : '');
    
        return $ret;
    }
    
    /**
     * Check the object is valid for use.
     * (This checks the object is valid and a database is open)
     *
     * @access  public
     * @return  boolean                 true if object is valid
     */
    function IsValid() {
        if (!is_a($this, 'tntdatabase')) {
            $this->_SetError('Not a valid database object');
            $ret = false;
        } else if (!$this->IsConnected()) {
            $this->_SetError('Not connected to database server');
            $ret = false;
        } else if (!$this->IsOpen()) {
            $this->_SetError('No open database');
            $ret = false;
        } else {
            $ret = true;
        }
        return $ret;
    }

    /**
     * Construct the object
     *
     * Supported construction options are;
     *   + mixed    'dsn'       - Data Source Name
     *   + string   'prefix'    - default table name prefix
     *   + string   'tables'    - associative array of table mappings
     *   + boolean  'persistant'- use a persistant connection
     *   + string   'cachePath' - path to use to cache database queries
     *
     * @access  public
     * @param   array       $options    construction options
     * @return  void
     */
    function TNTDatabase($options = null) {

        TNTObject::TNTObject();

        if (!empty($options['cachePath'])) {
            $this->cachePath = $options['cachePath'];
        }

        if (!empty($options['dsn'])) {
            // Try to connect to the specified database server
            $dsn = $options['dsn'];
            unset($options['dsn']);
            if (!$this->Connect($dsn, $options)) {
                TNT::Abort('TNTDatabase::TNTDatabase() - failed to connect to the specified database server: '.$dsn);
            }
        }

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTDatabase() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
