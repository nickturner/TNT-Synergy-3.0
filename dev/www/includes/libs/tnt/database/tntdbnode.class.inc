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
 * Database Node Access Class
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class is a extension to the basic {@link TNTDbRec} class to provide
 * a series of methods for manipulating hierachical database records (tree
 * nodes) as associative arrays.
 *
 * @package TNT.Database
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTDbRec');

/*
 * Globals
 */

/*
 * Defines
 */
define('TNT_DBNODE_POS_CHILD',      'child');
define('TNT_DBNODE_POS_SIBLING',    'sibling');
define('TNT_DBNODE_POS_SIBLING_REL','sibling_rel');

define('TNT_DBNODE_POS_MIN',        0);
define('TNT_DBNODE_POS_MAX',        0x777F);

/**
 * Database Node Position Structure
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @package TNT.Database
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTDbNodePos {
    /**
     * Key value of this nodes parent.
     * @var     scalar                  parent nodes key value
     * @access  public
     */
    var $pkv = null;

    /**
     * Number of this node under the given parent (0 = first)
     * @var     string                  child number (null if not supported)
     * @access  public
     */
    var $cnv = null;

    /**
     * Construct the object
     *
     * @access  public
     * @param   scalar      $pkv        parent nodes key value
     * @param   integer     $cnv        child number
     * @return  TNTDbNodePos&           reference to a new object
     */
    function TNTDbNodePos($pkv, $cnv) {
        $this->pkv = $pkv;
        $this->cnv = $cnv;
    }
}

/**
 * Database Node Access Class
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class is a extension to the basic {@link TNTDbRec} class to provide
 * a series of methods for manipulating hierachical database records (tree
 * nodes) as associative arrays.
 *
 * In addition to {@link TNTDbRec} this class supports the concept of each
 * record being a node in a hierachical database. The 'Adjacency List Model'
 * is used to create the hierachy where each record has a foriegn key field
 * referencing its parent record.
 *
 * Each record is assumed to contain the following control fields;
 *   Mandatory fields
 *     'rkf' - a field holding the nodes unique key
 *     'pkf' - a field holding the nodes parent key
 *   Optional fields
 *     'tif' - a field holding a tree identifier (allows multiple trees)
 *     'ndf' - a field holding the nodes depth from the root
 *     'cnf' - a field holding the nodes order under its parent (0 = left)
 *
 * This model is good for small tree's, however obtaining whole branches
 * of the tree require recursion and a separate database query for each node.
 * This causes such accesses to be slow compared to other more advanced tree
 * models.
 *
 * The actual root node is virtual and not stored in the database, the first
 * nodes stored are the children of these nodes. These root children are
 * indicated using a special value for the parent key field (usually 0). This
 * value can be set in the constructor.
 *
 * If a real root node is required then it is best to create a single root
 * child and regard that as the root node.
 *
 * Consider the path '/blogs/fred', 'blogs' is a child of the root node and
 * not the id of the actual root node, hence the actual root node need only
 * be a virtual concept.
 *
 * The optional fields will be maintained by the class if specified, and serve
 * the following purposes;
 *
 *   The 'tif' field allows multiple trees to be stored in the same table, it
 *   is used to identify which nodes belong to which trees. If not specified
 *   then all nodes are assumed to make up a single tree. The tree that the
 *   methods should operate on is given using the {@link SetTreeId()} and
 *   {@link GetTreeId()}. 
 *
 *   The 'ndf' field allows the nodes depth in the hierachy to be stored with
 *   the node. Even without this field the nodes depth can be returned by
 *   requesting the psuedo field '_depth' in the methods that take a field
 *   list.
 *
 *   The 'cnf' field allows an order to be given to a nodes children. When
 *   inserting a node its position under a parent can be given. This should be
 *   an integer field and counts the nodes position starting from zero.
 *
 *
 * @package TNT.Database
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTDbNode extends TNTDbRec {

    /**
     * Field name of the parent foreign key field
     * @var     string                  database column name
     * @access  private
     */
    var $_pkf = 'pid';

    /**
     * Field name of the tree identification field
     * @var     string                  database column name
     * @access  private
     */
    var $_tif = null;

    /**
     * Field name of the node depth field
     * @var     string                  database column name
     * @access  private
     */
    var $_ndf = null;

    /**
     * Field name of the child order field
     * @var     string                  database column name
     * @access  private
     */
    var $_cnf = null;

    /**
     * Value given to parent key field to indicate the record is a root child
     * @var     scalar                  record key value
     * @access  private
     */
    var $_nkv = 0;

    /**
     * Value for Tree Identification field
     * @var     scalar                  identification value
     * @access  private
     */
    var $_tiv = null;

    /**
     * Tree identification value as an SQL AND expression
     * @var     string                  "AND `{$this->_tif}` = {$this->_tiv}"
     * @access  private
     */
    var $_tivSql = null;

    /**
     * Reserved fields (these are the names of the class members holding the
     * names of reserved fields that can not be altered using {@link Modify()}
     * or {@link Change()} methods)
     * @var     string[]                class member names
     * @access  private
     */
    var $_resFlds = array('_rkf', '_pkf', '_tif', '_ndf', '_cnf');

    /**
     * Extra fields (these are fields that can be requested in addition to
     * the schema fields). They are usually calculated fields such as the
     * nodes depth, etc..
     * @var     string[]                array of psuedo column names
     */
    var $_xFlds = array('_depth');

    /**
     * Called just before and after a batch of records are created.
     *
     * If the method returns false then the triggering method will fail. This
     * method should call {@link _SetError()} to indicate why it failed.
     *
     * If the after event is failed then the create will have been done so it
     * is up to this method to reverse the create before returning failed.
     *
     * @access  private
     * @param   boolean     $pre        true if called before SQL INSERT
     * @param   array&      $recs       reference to array of records created
     * @param   TNTDbNodePos $pos       position records are being created in
     * @return  boolean                 true if successfull
     */
    function _OnCreateTree($pre, &$recs, $pos) {
        return true;
    }

    /**
     * Process returned database records.
     *
     * Helper function.
     *
     * @access  private
     * @param   array[]     $recs       returned records
     * @param   string[]    $flds       field names requested
     * @param   scalar      $exclude    exclude any record with this key
     * @return  array                   processed result
     */
    function _ProcessRecs($recs, $flds, $exclude = null) {
        if (is_null($recs)) {
            return null;
        }

        if (($max_i = count($recs)) == 0) {
            return array();
        }

        $fld = ($flds ? (count($flds) == 1 ? $flds[0] : null) : null);

        if ($fld == '*') {
            $fld = null;
        }

        $ret = array();
        for ($i = 0; $i < $max_i; $i++) {
            $key = $recs[$i][$this->_rkf];
            if (!is_null($exclude) && $key == $exclude) {
                continue;
            }
            if (!$flds) {
                // Return numerical indexed array of primary key values
                $ret[] = $key;
            } else if ($fld) {
                // Return the fields value indexed by the primary key value
                $ret[$key] = $recs[$i][$fld];
            } else {
                // Return the record indexed by the primary key value
                $ret[$key] = $recs[$i];
            }
        }

        return $ret;
    }

    /**
     * Process records in to tree order.
     *
     * Helper function.
     *
     * @access  private
     * @param   scalar      $key        parent key of record
     * @param   string      $recs       reference to records to select from
     * @param   integer     $depth      depth of recursion
     * @param   array       $tree       reference to where to store tree
     * @return  boolean                 true if successfull
     */
    function _PreOrderTreeSort($key, &$recs, $max, $depth, &$tree) { 

/*
        for ($i = 0; $i < $max; $i++) {
            if ($recs[$i][$this->_pkf] == $key) {
                if ($showdepth) {
                    $recs[$i]['_depth'] = $depth;
                }
                $tree[] = $recs[$i];

                // Recursively work on this child
                $this->_PreOrderTreeSort($recs[$i][$this->_rkf], $recs, $max, $depth + 1, $showdepth, $tree);
            }
        }
*/

        // this method is about twice as fast as above as it removes items from
        // the $recs as they are added to the $tree.
        // NB: Two loops are used as you can't recursively nest foreach() loops
        // on the referenced $recs array.
        $children = null;

        foreach ($recs as $i => $rec) {
            if ($rec[$this->_pkf] == $key) {
                if (isset($rec['_depth']) && $rec['_depth'] == -1) {
                    $rec['_depth'] = $depth;
                }
                $children[] = $rec;
                unset($recs[$i]);
            }
        }

        if ($children) {
            foreach ($children as $rec) {
                $tree[] = $rec;
                // Recursively work on this child
                $this->_PreOrderTreeSort($rec[$this->_rkf], $recs, $max, $depth + 1, $tree);
            }
        }

        return true;
    }

    /**
     * Process records into Tree Grid format.
     *
     * Helper function.
     *
     * @access  private
     * @param   array       $recs       records to select from
     * @param   scalar      $key        key of exported root node
     * @param   string      $fmt        format ('grid')
     * @return  array                   bi-dimensional tree grid
     */
    function _TreeExportGrid($recs, $key, $fmt = 'grid') {
        if ($fmt != 'grid') {
            $this->_SetError('Format must be \'grid\'');
            return null;
        }

        $ret = array();
        $max = count($recs);
        for ($i = 0; $i < $max; $i++) {
            $rec = $recs[$i];
            if (!isset($tree[$rec[$this->_pkf]])) {
                $tree[$rec[$this->_pkf]] = array();
            }
            $ret[$rec[$this->_pkf]][$rec[$this->_rkf]] = $rec;
        }

        return $ret;
    }

    /**
     * Process records from Tree Grid format.
     *
     * Any control fields in the existing data are overwritten using the
     * correct values according to the data structure.
     *
     * A nodes depth can not be calculated from the grid so any depth field
     * is set to -1.
     *
     * Parent records must appear before child records in the grid.
     *
     * Helper function.
     *
     * @access  private
     * @param   array       $data       bi-dimensional tree grid
     * @param   string      $fmt        format ('grid')
     * @param   array       $pos        records position in hierachy (or null)
     * @return  array                   array of records
     */
    function _TreeImportGrid($data, $fmt = 'grid', $pos = null) {
        if ($fmt != 'grid') {
            $this->_SetError('Format must be \'grid\'');
            return null;
        }
        $ret = array();
        foreach ($recs as $pk => $pr) {
            $cnf = 0;
            foreach ($pr as $k => $r) {
                $r[$this->_pkf] = $pk;
                if ($this->_tif) {
                    $r[$this->_tif] = $this->_tiv;
                }
                if ($this->_ndf) {
                    $r[$this->_ndf] = -1;
                }
                if ($this->_cnf) {
                    $r[$this->_cnf] = $cnf++;
                }
                $ret[] = $r;
            }
        }
        return $ret;
    }

    /**
     * Scan a list of record of all children of $pid.
     *
     * This method works recusively.
     *
     * Helper function.
     *
     * @access  private
     * @param   array       $rec        reference to record to expand
     * @param   array       $recs       reference to records to select from
     * @param   integer     $max        number of records
     * @param   integer     $depth      recursion depth
     * @return  boolean                 true if successfull
     */
    function _ContractNode(&$rec, &$recs, $max = null, $depth = 0) {

        $rec['_numchildren'] = 0;
        $rec['_numdescendants'] = 0;
        $rec['_children'] = array();

        $rkv = $rec[$this->_rkf];

        // Gather up all the children and remove them from the pool
        foreach ($recs as $i => $r) {
            if ($r[$this->_pkf] == $rkv) {
                $r['_depth'] = $depth;
                $r['_parent'] =& $rec;
                $rec['_children'][] = $r;
                $rec['_numchildren']++;
                unset($recs[$i]);
            }
        }

        $rec['_numdescendants'] = $rec['_numchildren'];

        // Repeat for each child
        foreach ($rec['_children'] as $i => $r) {
            if (!$this->_ContractNode($rec['_children'][$i], $recs, $max, $depth + 1)) {
                return false;
            }
            $rec['_numdescendants'] += $rec['_children'][$i]['_numdescendants'];
        }

        return true;
    }

    /**
     * Expand children
     *
     * This method works recusively.
     *
     * Helper function.
     *
     * @access  private
     * @param   array       $rec        record to expand
     * @param   array       $recs       reference to expanded records
     * @param   integer     $depth      recursion depth
     * @return  boolean                 true if successfull
     */
    function _ExpandNode($rec, &$recs, $depth = 0) {
        static $_key = 1;

        if (isset($rec[$this->_rkf])) {
            $rkv = $rec[$this->_rkf];
        } else {
            $rkv = $rec[$this->_rkf] = $_key++;
        }

        if (!isset($rec[$this->_pkf])) {
            $rec[$this->_pkf] = $this->_nkv;
        }

        if ($this->_tif) {
            $rec[$this->_tif] = $this->_tiv;
        }

        if ($this->_ndf) {
            $rec[$this->_ndf] = $depth;
        }

        $children = null;
        if (isset($rec['_children']) && is_array($rec['_children']) && count($rec['_children'])) {
            // The test for integer key allows _children to be the child or an
            // array of children. Thats becuase the XML parser will not give
            // an array where only one child exists.
            reset($rec['_children']);
            if (!is_integer(key($rec['_children']))) {
                $children[] = $rec['_children'];
            } else {
                $children = $rec['_children'];
            }
            unset($rec['_children']);
        }

        $recs[] = $rec;

        if ($children) {
            $cnf = 0;
            foreach ($children as $r) {
                $r[$this->_pkf] = $rkv;
                if ($this->_cnf) {
                    $r[$this->_cnf] = $cnf++;
                }
                if (!$this->_ExpandNode($r, $recs, $depth + 1)) {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * Process records into Tree node format.
     *
     * Helper function.
     *
     * @access  private
     * @param   array       $recs       records to select from
     * @param   scalar      $key        key of exported root node
     * @param   string      $fmt        format ('node')
     * @return  array                   array of node formated records
     */
    function _TreeExportNode($recs, $key, $fmt = 'node') {
        if ($fmt != 'node') {
            $this->_SetError('Format must be \'node\'');
            return null;
        }

        $max = count($recs);

        for ($i = 0; $i < $max; $i++) {
            if (isset($recs[$i])) {
                if (!$this->_ContractNode($recs[$i], $recs, $max)) {
                    return null;
                }
            }
        }

        $ret = array();

        // Return an array of references to the root nodes
        foreach ($recs as $i => $rec) {
            if (!isset($rec['_parent'])) {
                $ret[] =& $recs[$i];
            }
        }

        return $ret;
    }

    /**
     * Process records from Tree node format.
     *
     * Helper function.
     *
     * @access  private
     * @param   array       $nodes      array of node formatted records
     * @param   string      $fmt        format ('node')
     * @param   array       $pos        records position in hierachy (or null)
     * @return  array                   array of records
     */
    function _TreeImportNode($nodes, $fmt = 'node', $pos = null) {
        if ($fmt != 'node') {
            $this->_SetError('Format must be \'node\'');
            return null;
        }

        $recs = null;
        foreach ($nodes as $node) {
            if (!$this->_ExpandNode($node, $recs)) {
                return null;
            }
        }

        return $recs;
    }

    /**
     * Process records in to Tree XML format.
     *
     * Helper function.
     *
     * @access  private
     * @param   array       $recs       records to select from
     * @param   scalar      $key        key of exported root node
     * @param   string      $fmt        format ('xml')
     * @return  string                  XML formatted record tree
     */
    function _TreeExportXml($recs, $key, $fmt = 'xml') {
        if ($fmt != 'xml') {
            $this->_SetError('Format must be \'xml\'');
            return null;
        }

        $ret = '';

        $tree = array();
        $this->_PreOrderTreeSort($key, $recs, count($recs), 0, $tree);

        $tag = $this->_table;

        // Finished with them
        $recs = null;

        $rec = reset($tree);
        $start_depth = $rec['_depth'];
        $roots = 0;

        while ($rec) {
            $depth = $rec['_depth'];

            $attrs = '';
            foreach ($rec as $f => $v) {
                if (is_scalar($v)) {
                    $attrs .= " $f=\"".htmlentities($v).'"';
                }
            }

            if ($depth == $start_depth) {
                $roots++;
            }

            $rec = next($tree);

            $new_depth = ($rec ? $rec['_depth'] : $start_depth);

            if ($new_depth == $depth) {
                $ret .= "<{$tag}{$attrs} />\n";
            } else if ($new_depth == $depth + 1) {
                $ret .= "<{$tag}{$attrs}>\n";
            } else if ($new_depth < $depth && $new_depth >= $start_depth) {
                $ret .= "<{$tag}{$attrs} />\n";
                while (--$depth >= $new_depth) {
                    $ret .= "</{$tag}>\n";
                }
            } else {
                $this->_SetError('Invalid record depth information');
                return null;
            }
        }

        if ($roots > 1) {
            // An XML tree can only have one root - so we wrap it in a
            // dummy root element
            $ret = "<tree>\n$ret\n</tree>\n"; 
        }

        return $ret;
    }

    /**
     * Process records from Tree XML format.
     *
     * Helper function.
     *
     * @access  private
     * @param   string      $xml        XML formatted record tree
     * @param   string      $fmt        format ('xml')
     * @param   array       $pos        records position in hierachy (or null)
     * @return  array                   array of records
     */
    function _TreeImportXml($xml, $fmt = 'xml', $pos = null) {
        if ($fmt != 'xml') {
            $this->_SetError('Format must be \'xml\'');
            return null;
        }

        $xml = str_replace( array("<{$this->_table}", "</{$this->_table}"),
                            array('<_children', '</_children'),
                            $xml);

        $dummy_root = (strncasecmp($xml, '<tree>', 6) == 0);

        TNTLoader::RequireClass('TNTXmlParser');

        $parser = new TNTXmlParser();

        if (!$parser->ParseData($xml, TNT_TAGPARSER_OPTS_SIMPLE)) {
            $this->_SetError('Invalid XML data', $parser);
            return null;
        }

        $data = $parser->Root();

        destroy($parser);

        if ($dummy_root) {
            $children = $data['tree']['_children'];
        } else {
            $children = $data['_children'];
        }

        reset($children);
        if (!is_integer(key($children))) {
            $children = array($children);
        }

        $recs = null;
        foreach ($children as $rec) {
            if (!$this->_ExpandNode($rec, $recs)) {
                return null;
            }
        }

        return $recs;
    }

    /**
     * Get the descendants of the given record.
     *
     * This is a recursive methid.
     *
     * Helper function.
     *
     * @access  private
     * @param   scalar      $key        key of record to get descendants of
     * @param   string      $sql        SQL template
     * @param   integer     $depth      depth of recursion
     * @param   array                   reference to resultant array of records
     * @return  boolean                 true if ok
     */
    function _DescendantsR($key, $sql, $depth = 0, &$recs) {

        $ret = false;

        // Start error section
        do {

            $rkv = $this->db->PrepareVal($key, $this->csf);

            $_sql = str_replace('{key}', $rkv, $sql);

            // Get all children of this record
            $recs1 = $this->db->Fetch($_sql, 0, TNT_DB_FETCH_ASSOC, $this->csf);
            if (is_null($recs1)) {
                $this->_SetError('Can not get children of record: '.$key, $this->db);
                break;
            }

            // Add the record and then the descendants of the record
            $max_i = count($recs1);
            for ($i = 0; $i < $max_i; $i++) {
                if (isset($recs1[$i]['_depth']) && $recs1[$i]['_depth'] == -1) {
                    $recs1[$i]['_depth'] = $depth;
                }
                $recs[] = $recs1[$i];
                if (!$this->_DescendantsR($recs1[$i][$this->_rkf], $sql, $depth + 1, $recs)) {
                    $this->_SetError('Can not get descendants of record: '.$rec[$this->_rkf], $this->db);
                    break;
                }
            }

            $ret = true;

        } while (false);
        /* End error section */
           
        return $ret;
    }

    /**
     * Return an array of all ancestor records.
     *
     * Returns an array of ancestor records, that is all records that sit above
     * the the specified record in the node tree.
     *
     * The '$key' argument should be the record key of a specific node record
     * whose ancestors should be returned.
     *
     * The '$fld' argument can be an array of field names or a comma separated
     * list of field names, in which case only the specified fields are fetched
     * for each record. The '*' field name can be used to mean all fields. The
     * '_depth' field name can be used to return the depth of each record from
     * the root node (which is at depth 0). The records key field is always
     * returned.
     *
     * The '$inc' argument can be used to cause the node specified by '$key' to
     * be included in the resultant records (even though it is not technically
     * an ancestor of itself).
     *
     * The '$ord' argument can be an array of field names or a comma separated
     * list of field names, in which case the records are ordered using those
     * fields. The field names can have the normal 'ASC' or 'DESC' post fix.
     * If a null sort order is specified then the child number field ($cnf) will
     * be used if defined, otherwise the natural database ordering will be used.
     *
     * The ancestors are returned from the root down.
     *
     * @access  public
     * @param   scalar      $key        key of record to get ancestors of
     * @param   string      $fld        name of field to return (null for none)
     * @param   boolean     $inc        include starting node in results
     * @return  array                   array of all ancestor nodes
     * @see     Ancestors()
     */
    function _Ancestors($key, $fld = null, $inc = false) {
    
        $ret = null;

        // Start error section
        do {
            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);
            $f = $flds;
            $f[] = $this->_rkf;
            $f[] = $this->_pkf;
            $f = join(',', $this->ProcessFlds($f));

            $id = $key;

            $recs = array();

            while ($id != $this->_nkv) {
                $rkv = $this->db->PrepareVal($id, $this->csf);

                timestamp('Fetching: '.$rkv);
                $recs1 = $this->db->Fetch("SELECT $f FROM `{$this->_tab}` WHERE `{$this->_rkf}` = $rkv {$this->_tivSql}", 0, TNT_DB_FETCH_ASSOC, $this->csf);
                timestamp('Fetched: '.$rkv);
                if (is_null($recs1)) {
                    $this->_SetError('Can not select record: '.$id, $this->db);
                    break;
                }
                if (count($recs1) != 1) {
                    $this->_SetError('No such record: '.$id);
                    return null;
                }

                $id = $recs1[0][$this->_pkf];
                
                array_unshift($recs, $recs1[0]);
            }

            if (!$inc) {
                array_pop($recs);
            }

            // Correct any depth field
            if (isset($recs[0]['_depth']) && $recs[0]['_depth'] == -1) {
                $max_i = count($recs);
                for ($i = 0; $i < $max_i; $i++) {
                    $recs[$i]['_depth'] = $i;
                }
            }

            $ret = $recs;

        } while (false);
        /* End error section */
    
        return $ret;
    }

    /**
     * Return an array of all descendant records.
     *
     * Returns an array of descendant records, that is all records that sit
     * below the the specified record in the node tree (all records that have
     * the specified record as an ancestor).
     *
     * The '$key' argument should be the record key of a specific node record
     * whose descendants should be returned. If you specify the virtual root
     * node key ($nkv) then you get the whole tree in a more optimised fashion
     * using a single database query.
     *
     * The '$fld' argument can be an array of field names or a comma separated
     * list of field names, in which case only the specified fields are fetched
     * for each record. The '*' field name can be used to mean all fields. The
     * '_depth' field name can be used to return the depth of each record from
     * the root node (which is at depth 0). The records key field is always
     * returned.
     *
     * The '$inc' argument can be used to cause the node specified by '$key' to
     * be included in the resultant records (even though it is not technically
     * a descendant of itself).
     *
     * The '$ord' argument can be an array of field names or a comma separated
     * list of field names, in which case the records are ordered using those
     * fields. The field names can have the normal 'ASC' or 'DESC' post fix.
     * If a null sort order is specified then the child number field ($cnf) will
     * be used if defined, otherwise the natural database ordering will be used.
     *
     * The '$pot' argument can be used to cause the return records to be sorted
     * as if the were returned doing a preorder traversal of the tree where
     * each nodes children are ordered using the '$ord' argument. This can be
     * quite slow if a lot of records are involved. The records can be sorted
     * into this order at a later time using the method
     * {@link _PreOrderTreeSort()}
     *
     * @access  public
     * @param   scalar      $key        key of record to get ancestors of
     * @param   mixed       $fld        name of field to return (null for none)
     * @param   boolean     $inc        include starting record in results
     * @param   array       $ord        sort order
     * @param   boolean     $pot        return in pre order traversal order
     * @return  array                   array of all descendant nodes
     * @see     Descendants()
     */
    function _Descendants($key, $fld = null, $inc = false, $ord = null, $pot = false) {
    
        $ret = null;

        // Start error section
        do {
            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);
            $f = $flds;
            $f[] = $this->_rkf;
            $f = join(',', $this->ProcessFlds($f));

            $o = (is_null($ord) ? ($this->_cnf ? "`{$this->_cnf}` ASC" : '') : join(',', $this->ProcessOrder($ord)));
            if ($o) {
                $o = "ORDER BY $o";
            }

            if ($key == $this->_nkv) {
                // Get the whole tree
                $f .= ",`{$this->_pkf}`";
                $w = ($this->_tif ? 'WHERE'.substr($this->_tivSql, 3) : '');

                $recs =  $this->db->Fetch("SELECT $f FROM `{$this->_tab}` $w $o", 0, TNT_DB_FETCH_ASSOC, $this->csf);
                if (is_null($recs)) {
                    $this->_SetError('Can not get whole record tree', $this->db);
                    break;
                }

                if ($pot) {
                    // Process the tree into heirachical order
                    $this->_PreOrderTreeSort($key, $recs, count($recs), 0, $ret);

                    // Remove the parent key field unless we asked for it.
                    $show_pkf = ($flds && ($flds[0] == '*' || in_array($this->_pkf, $flds)));
                    if (!$show_pkf) {
                        $max_i = count($ret);
                        for ($i = 0; $i < $max_i; $i++) {
                            unset($ret[$i][$this->_pkf]);
                        }
                    }

                } else {
                    $ret = $recs;
                }

                break;
            }

            $depth = -1;

            if ($inc) {
                // Get this record
                $rkv = $this->db->PrepareVal($key, $this->csf);

                $recs = $this->db->Fetch("SELECT $f FROM `{$this->_tab}` WHERE `{$this->_rkf}` = $rkv {$this->_tivSql}", 0, TNT_DB_FETCH_ASSOC, $this->csf);
                if (is_null($recs)) {
                    $this->_SetError('Can not select record: '.$key, $this->db);
                    break;
                }
                if (count($recs) != 1) {
                    $this->_SetError('Can not select record: '.$key);
                    break;
                }

                // Correct any depth field
                if (isset($recs[0]['_depth'])) {
                    if ($recs[0]['_depth'] == -1) {
                        $recs[0]['_depth'] = $this->Depth($key);
                    }
                    $depth = $recs[0]['_depth'] + 1;
                }
            } else {
                if (strpos($f, '_depth')) {
                    $depth = $this->Depth($key) + 1;
                }
                $recs = array();
            }

            $rkv = $this->db->PrepareVal($key, $this->csf);

            $sql = "SELECT $f FROM `{$this->_tab}` WHERE `{$this->_pkf}` = {key} {$this->_tivSql} $o";

            // Get all children of this record
            if (!$this->_DescendantsR($key, $sql, $depth, $recs)) {
                $this->_SetError('Can not get descendants of record: '.$key, $this->db);
                break;
            }

            $ret = $recs;

        } while (false);
        /* End error section */
    
        return $ret;
    }

    /**
     * Return an array of all sibling records.
     *
     * Returns an array of sibling records, that is all record that have the
     * same parent as the specified record.
     *
     * The '$key' argument should be the record key of a specific node record
     * whose descendants should be returned. If you specify the virtual root
     * node key ($nkv) then you get the whole tree in a more optimised fashion
     * using a single database query.
     *
     * The '$fld' argument can be an array of field names or a comma separated
     * list of field names, in which case only the specified fields are fetched
     * for each record. The '*' field name can be used to mean all fields. The
     * '_depth' field name can be used to return the depth of each record from
     * the root node (which is at depth 0). The records key field is always
     * returned.
     *
     * The '$inc' argument can be used to cause the node specified by '$key' to
     * be included in the resultant records (even though it is not technically
     * a descendant of itself).
     *
     * The '$ord' argument can be an array of field names or a comma separated
     * list of field names, in which case the records are ordered using those
     * fields. The field names can have the normal 'ASC' or 'DESC' post fix.
     * If a null sort order is specified then the child number field ($cnf) will
     * be used if defined, otherwise the natural database ordering will be used.
     *
     * @access  public
     * @param   scalar      $key        key of record to get ancestors of
     * @param   mixed       $fld        name of field to return (null for none)
     * @param   boolean     $inc        include starting record in results
     * @param   array       $ord        sort order
     * @return  array                   array of all descendant nodes
     * @see     Siblings()
     */
    function _Siblings($key, $fld = null, $inc = false, $ord = null) {
    
        $ret = null;

        // Start error section
        do {
            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);
            $f = $flds;
            $f[] = $this->_rkf;
            $f = join(',', $this->ProcessFlds($f, 'r1'));

            $o = (is_null($ord) ? ($this->_cnf ? "r1.`{$this->_cnf}` ASC" : '') : join(',', $this->ProcessOrder($ord, 'r1')));
            if ($o) {
                $o = "ORDER BY $o";
            }

            $tivSql = '';
            if ($this->_tif) {
                $tivSql = preg_replace('/^AND /', 'AND r1.', $this->_tivSql);
                $tivSql .= preg_replace('/^AND /', ' AND r2.', $this->_tivSql);
            }

            $rkf = $this->db->PrepareVal($key, $this->csf);

            $incSql = '';
            if (!$inc) {
                $incSql = "AND r1.`{$this->_rkf}` != $rkf";
            }

            // Get immediate children of our parent
            $recs = $this->db->Fetch("SELECT $f FROM `{$this->_tab}` r1, `{$this->_tab}` r2 WHERE r1.`{$this->_pkf}` = r2.`{$this->_pkf}` AND r2.`{$this->_rkf}` = $rkf {$incSql} {$tivSql} $o", 0, TNT_DB_FETCH_ASSOC, $this->csf);

            if (is_null($recs)) {
                $this->_SetError('Can not select siblings of record: '.$key, $this->db);
                break;
            }

            if (isset($recs[0]['_depth']) && $recs[0]['_depth'] == -1) {
                $depth = $this->Depth($key);
                $max_i = count($recs);
                for ($i = 0; $i < $max_i; $i++) {
                    $recs[$i]['_depth'] = $depth;
                }
            }

            $ret = $recs;
           
        } while (false);
        /* End error section */
    
        return $ret;
    }

    /**
     * Make space for a new tree branch at given position.
     *
     * This method will reposition the existing children of the specified
     * record so that a branch of nodes can be inserted at the given position.
     *
     * @access  public
     * @param   array       $pos        records position in hierachy (or null)
     * @return  boolean                 true if space made
     */
    function _MakeSpace($pos) {

        $ret = false;

        // Start error section
        do {

            if (!$this->_cnf) {
                $ret = true;
                break;
            }

            $max = $this->ChildCount($pos->pkv);
            if (is_null($max)) {
                break;
            }

            // If child count is past maximum then no need to move any children
            if ($pos->cnv > $max) {
                $ret = true;
                break;
            }

            // Move the children after position up by 1 to make gap for branch
            $pkv = $this->db->PrepareVal($pos->pkv, $this->csf);
            if (!$this->db->Query("UPDATE `{$this->_tab}` SET `{$this->_cnf}` = `{$this->_cnf}` + 1 WHERE `{$this->_cnf}` >= {$pos->cnv} AND `{$this->_pkf}` = $pkv")) {
                $this->_SetError('Can not re-order children of node: '.$pkv, $this->db);
                break;
            }

            if (isset($this->_childCount[$pos->pkv])) {
                $this->_childCount[$pos->pkv]++;
            }
            if (isset($this->_descendantCount[$pos->pkv])) {
                $this->_descendantCount[$pos->pkv]++;
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }

    /**
     * Translate a field.
     *
     * An optional table name can be prefixed to the fields.
     *
     * The field name must be one of the tables fields or one of the special
     * field names as follows;
     *   + '_depth' - returns the depth of the node (or -1 if unknown)
     *
     * Any other fields starting with '_' are return as null and should be
     * ignored.
     *
     * Helper function.
     *
     * @access  private
     * @param   string      $fld        field name
     * @param   string      $pfx        prefix to add before field name
     * @return  string                  SQL select column
     */
    function _TranslateFld($fld, $table = null) {
        $ret = null;
        if ($fld{0} == '_') {
            if ($fld == '_depth') {
                if ($this->_ndf) {
                    $ret = ($table ? "`{$table}`." : '')."`{$this->_ndf}` AS _depth";
                } else {
                    $ret = '-1 AS _depth';
                }
            }
        } else {
            $ret = ($table ? "`{$table}`." : '')."`{$fld}`";
        }
        return $ret;
    }

    /**
     * Process a field list.
     *
     * An optional table name can be prefixed to the fields.
     *
     * The optional table name and the field names are quoted for use with
     * MySQL incase they are reserved names.
     *
     * The field name '*' means all fields and if present it is expanded to
     * be a list of the records field names. This expansion occurs so that
     * other fields can be added as '*,fld' is invalid SQL syntax. All fields
     * means all listed in the private member variable $this->_flds.
     *
     * The method returns an associative array of full escaped field names
     * indexed by the raw field name.
     *
     * Helper function.
     *
     * @access  private
     * @param   mixed       $flds       array of or csv string of field names
     * @param   string      $pfx        prefix to add before field name
     * @return  array                   array of SQL field names index by name
     */
    function ProcessFlds($flds, $table = null) {
        if (!$flds) {
            return null;
        }

        if (!is_array($flds)) {
            $flds = explode(',', $flds);
        }

        $ret = array();

        foreach ($flds as $f) {
            if ($f == '*') {
                foreach (array_merge($this->_flds, $this->_xFlds) as $f) {
                    if (!isset($ret[$f])) {
                        if (isset($this->__translatedFlds[$f.$table])) {
                            $ret[$f] = $this->__translatedFlds[$f.$table];
                        } else if ($tf = $this->_TranslateFld($f, $table)) {
                            $this->__translatedFlds[$f.$table] = $tf;
                            $ret[$f] = $tf;
                        }
                    }
                }
            } else {
                if (!isset($ret[$f])) {
                    if (isset($this->__translatedFlds[$f.$table])) {
                        $ret[$f] = $this->__translatedFlds[$f.$table];
                    } else if ($tf = $this->_TranslateFld($f, $table)) {
                        $this->__translatedFlds[$f.$table] = $tf;
                        $ret[$f] = $tf;
                    }
                }
            }
        }

        return $ret;
    }

    /**
     * Process an order list.
     *
     * The input is a list of fields optionally postfixed with sort order
     * extension ('ascending|descending' or abbr thereof).
     *
     * An optional table name can be prefixed to the fields.
     *
     * The optional table name and the field names are quoted for use with
     * MySQL incase they are reserved names.
     *
     * The method returns an associative array of full escaped field names
     * with their order appended indexed by the raw field name.
     *
     * The method does not validate the sort order extension is a valid
     * order.
     *
     * Helper function.
     *
     * @access  private
     * @param   mixed       $flds       array of or csv string of field names
     * @param   string      $pfx        prefix to add before field name
     * @return  array                   array of SQL field names index by name
     */
    function ProcessOrder($flds, $table = null) {
        if (!$flds) {
            return null;
        }

        if (!is_array($flds)) {
            $flds = explode(',', $flds);
        }

        $ret = array();

        foreach ($flds as $f) {
            $a = explode(' ', $f, 2);
            $f = $a[0];
            $o = (isset($a[1]) ? " {$a[1]}" : '');
            if (!isset($ret[$f])) {
                $ret[$f] = ($table ? "`{$table}`.`{$f}`{$o}" : "`{$f}`{$o}");
            }
        }

        return $ret;
    }

    /**
     * Return the parent key field column name.
     *
     * This name is set in the object constructor and can not change for the
     * life of the object.
     *
     * @access  public
     * @return  string                  database column name of parent key
     */
    function ParentKeyName() {
        return $this->_pkf;
    }

    /**
     * Return the order field column name.
     *
     * This name is set in the object constructor and can not change for the
     * life of the object.
     *
     * The order field is optional and if not set then no child ordering is
     * supported.
     *
     * @access  public
     * @return  string                  database column name of order field
     */
    function OrderFldName() {
        return $this->_cnf;
    }

    /**
     * Create a record in the database
     *
     * A reference to the record should be given so that the timestamps can be
     * updated. Also any auto increment primary key is updated in the record.
     *
     * This method differs from the base {@link TNTDbRec::Create()} method in
     * that the record position in the tree can be specified.
     *
     * If no position is provided then any positioning information in the
     * record is used.
     *
     * The position information can be the result of a call to the method
     * {@link AbsPos()}.
     *
     * When the method is called as part of importing a whole tree branch
     * then the root branch is called with the correct $pos and subsequent
     * branches are created with $pos set to -1. This means that the pos
     * need not be checked and space need not be made for it.
     *
     * @access  public
     * @param   array       $rec        reference to structure defining record
     * @param   array       $pos        records position in hierachy (or null)
     * @return  boolean                 true if updated record successfully
     */
    function Create(&$rec, $pos = null) {
    
        $ret = false;

        // Start error section
        do {
            if (!$rec || !is_array($rec)) {
                $this->_SetError('Invalid record');
                break;
            }

            if ($pos != -1) {
                if ($pos) {
                    if (!$this->ChkPos($pos)) {
                        break;
                    }
                    $rec[$this->_pkf] = $pos->pkv;
                    if ($this->_cnf) {
                        $rec[$this->_cnf] = $pos->cnv;
                    }
                } else {
                    $pos->pkv = $rec[$this->_pkf];
                    if ($this->_cnf) {
                        $pos->cnv = $rec[$this->_cnf];
                    }
                    if (!$this->ChkPos($pos)) {
                        break;
                    }
                }
                if (!$this->_MakeSpace($pos)) {
                    break;
                }
            }
    
            if ($this->_tif) {
                $rec[$this->_tif] = $this->_tiv;
            }

            if ($this->_ndf) {
                $rec[$this->_ndf] = (($rec[$this->_pkf] == $this->_nkv) ? 0 : $this->Depth($rec[$this->_pkf]) + 1);
            }

            if (!parent::Create($rec)) {
                break;
            }

            // Update the depth cache while we know this new record depth
            if ($this->_ndf) {
                $this->__depth[$rec[$this->_rkf]] = $rec[$this->_ndf];
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }

    /**
     * Create a tree of records in the database
     *
     * A reference to the record should be given so that the timestamps can be
     * updated. Also any auto increment primary key is updated in the record.
     *
     * If no position is provided then any positioning information in the
     * record is used.
     *
     * The position information can be the result of a call to the method
     * {@link AbsPos()}.
     *
     * The records should be in preorder tranversal, that is the root comes
     * first and then each parent preceeds its children. The record and parent
     * record key fields are initialised to dummy integer values using the
     * correct relationships.
     *
     * There may be multiple root records, however the first record must always
     * be a root record.
     *
     * When the method is called as part of importing a whole tree branch
     * then the root branch is called with the correct $pos and subsequent
     * branches are created with $pos set to -1. This means that the pos
     * need not be checked and space need not be made for it.
     *
     * @access  public
     * @param   array       $rec        reference to structure defining record
     * @param   array       $pos        records position in hierachy (or null)
     * @return  boolean                 true if updated record successfully
     */
    function CreateTree(&$recs, $pos) {
    
        $ret = null;

        // Start error section
        do {

            if (!$recs || !is_array($recs)) {
                $this->_SetError('Invalid records');
                break;
            }

            if (!($max_i = count($recs))) {
                $ret = true;
                break;
            }

            $max_i = count($recs);

            // Trigger the pre create events
            if (!$this->_OnCreateTree(TNT_DBREC_PRE, $recs, $pos)) {
                break;
            }

            $rootid = $recs[0][$this->_pkf];

            // Loop around creating each record
            $ok = true;
            for ($i = 0; $i < $max_i; $i++) {
                $rec =& $recs[$i];
                if (!isset($rec[$this->_rkf]) ||
                    !isset($rec[$this->_pkf])) {
                    $this->_SetError('Invalid record: '.var_info($recs[$i]));
                    $ok = false;
                    break;
                }

                if ($this->_tif) {
                    $rec[$this->_tif] = $this->_tiv;
                }

                $id = $rec[$this->_rkf];
                $pid = $rec[$this->_pkf];

                if ($pid == $rootid) {

                    if (!$this->Create($rec, $pos)) {
                        $ok = false;
                        break;
                    }

                    $idmap[$pid] = $rec[$this->_pkf];
            //        if ($this->_ndf) {
            //            $ndmap[$pid] = $rec[$this->_ndf]; 
            //        }
                } else {
                    $rec[$this->_pkf] = $idmap[$pid];

            //       if ($this->_ndf) {
            //           $rec[$this->_ndf] = $ndmap[$id] = $ndmap[$pid] + 1; 
            //       }

                     if (!$this->Create($rec, -1)) {
                        $ok = false;
                        break;
                    }
                }

                $idmap[$id] = $rec[$this->_rkf];

                //if ($this->_ndf) {
                //    $this->__depth[$rec[$this->_rkf]] = $ndmap[$id];
                //}
            }

            // Trigger the post create events
            if (!$this->_OnCreateTree(TNT_DBREC_PST, $recs, $pos)) {
                break;
            }

            $ret = $ok;

        } while (false);
        /* End error section */
    
        return $ret;
    }
    
    /**
     * Modify a record in the database.
     *
     * A reference to the record should be given so that the modification
     * timestamp can be updated.
     *
     * The position of a node can not be modified using this method. To
     * reposition a node use {@link SetPos()}.
     *
     *
     * @access  public
     * @param   array       $rec        reference to structure defining record
     * @return  boolean                 true if modified record successfully
     */
    function Modify(&$rec) {
    
        $ret = false;

        // Start error section
        do {
            if (!$rec || !is_array($rec)) {
                $this->_SetError('Invalid record');
                break;
            }

            // Backup the reserved fields and remove them from the record
            $bak = null;
            foreach ($this->_resFlds as $f) {
                if ($f != '_rkf' && isset($rec[$f])) {
                    $bak[$f] = $rec[$f];
                    unset($rec[$f]);
                }
            }

            $ret = parent::Modify($rec);

            // Restore the backed up fields
            if ($bak) {
                foreach ($bak as $f => $v) {
                    $rec[$f] = $v;
                }
            }

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /**
     * Delete record(s) from the database
     *
     * The selection criteria are specified as follows;
     *
     * a) You can specify a scalar primary key value of a specific record.
     *
     * b) You can specify an array of scalar primary key values of a specific
     *    records.
     *
     * c) You can specify null or an empty array or the string '*' to mean
     *    all records.
     *
     * d) You can specify the string value 'WHERE <SqlCondition>' to specify a
     *    SQL where condition, the condition can refer to any field in the
     *    table using its real column name.
     *
     * The strict argument when set will cause the function to return false
     * unless all the records requested for deletion actualy exist. Without
     * this asking to delete non existant records is not an error. 
     *
     * The strict value is ignored if the selection criteria selects all records
     * or uses a 'WHERE' condition.
     *
     * If you specify the virtual root node key ($nkv) then you can delete
     * the whole tree, in the same way as using a selection criteria that
     * selects all records. However to do this the root node key must be
     * specified directly as the selection criteria and not included in an
     * array of keys or in a WHERE condition.
     *
     * This method differs from the base {@link TNTDbRec::Delete()} method in
     * that all children of the specified records are deleted. Deleting all
     * records is fast, deleting a selection of records is slow depending on
     * how many descendants each select record has.
     *
     * @access  public
     * @param   mixed       $sel        selection criteria
     * @param   boolean     $strict     fail unless all selected records deleted
     * @return  boolean                 true if deleted record(s) successfully
     */
    function Delete($sel, $strict = false) {
    
        $ret = false;

        // Start error section
        do {

            if ($sel === $this->_nkv) {
                $sel = '*';
            }

            $num = -1;

            $where = $this->PrepareSel($sel, $num);

            if (is_null($where)) {
                break;
            }

            if ($where == '') {
                // Special case (delete all records)

                $where = ($this->_tif ? 'WHERE'.substr($this->_tivSql, 3) : '');

                $ret = parent::Delete($where, $strict);

                break;
            }

            // Get details of all nodes being deleted
            $f = "{$this->_rkf}".($this->_cnf ? ", `{$this->_pkf}`, `{$this->_cnf}`" : '');
            $recs = $this->db->Fetch("SELECT $f FROM `{$this->_tab}` $where {$this->_tivSql}", 0, TNT_DB_FETCH_ASSOC, $this->csf);
            if (is_null($recs)) {
                $this->_SetError('Can not find records requiring deleting', $this->db);
                break;
            }
            if ($strict && $num != -1 && count($recs) != $num) {
                $this->_SetError('Can not find all records requiring deleting');
                break;
            }

            if (count($recs) == 0) {
                // No records to delete
                $ret = true;
                break;
            }

            // Get the ids of all descendants of the records being deleted
            // This is probably very slow !!!
            $ok = true;
            $ids = array();
            foreach ($recs as $rec) {
                $id = $rec[$this->_rkf];
                $ids[] = $id;
                $descendant_recs = $this->_Descendants($id);
                if (is_null($descendant_recs)) {
                    $this->_AddError('Can not find all descendants of record: '.$id);
                    $ok = false;
                    break;
                }
                foreach ($descendant_recs as $sr) {
                    $ids[] = $sr[$this->_rkf];
                }
            }
            if (!$ok) {
                break;
            }

            $ids = array_unique($ids);

            // Delete all the records we found
            $_ids = join(',', $ids);
            $num = count($ids);

            $where = "WHERE `{$this->_rkf}` IN ($_ids) {$this->_tivSql}";

            if (!parent::Delete($where, false)) {
                break;
            }

            if ($strict && $num != -1 && $this->db->NumAffectedRows() != $num) {
                $this->_SetError('Did not delete all selected records: '.$_ids);
                break;
            }

            // Re-position siblings of deleted records
            if ($this->_cnf) {
                foreach ($recs as $rec) {
                    $pid = $rec[$this->_pkf];
                    $ord = (int)$rec[$this->_cnf];
                    if (!in_array($pid, $ids)) {
                        $_pkv = $this->db->PrepareVal($pid, $this->csf);
                        if (!$this->db->Query("UPDATE `{$this->_tab}` SET `{$this->_cnf}` = `{$this->_cnf}` - 1 WHERE `{$this->_cnf}` >= $ord AND `{$this->_pkf}` = $_pkv {$this->_tivSql}")) {
                            $this->_SetError('Can not re-position children of node: '.$pid, $this->db);
                            break;
                        }
                        if (isset($this->_childCount[$pid])) {
                            $this->_childCount[$pid]++;
                        }
                    }
                }
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /**
     * Change record(s) in the database
     *
     * The selection criteria are specified as follows;
     *
     * a) You can specify a scalar primary key value of a specific record.
     *
     * b) You can specify an array of scalar primary key values of a specific
     *    records.
     *
     * c) You can specify null or an empty array or the string '*' to mean
     *    all records.
     *
     * d) You can specify the string value 'WHERE <SqlCondition>' to specify a
     *    SQL where condition, the condition can refer to any field in the
     *    table using its real column name.
     *
     * The strict argument when set will cause the function to return false
     * unless all the records requested for deletion actualy exist. Without
     * this asking to delete non existant records is not an error. 
     *
     * The strict value is ignored if the 'ALL' or 'WHERE' keywords are used.
     *
     * If you specify the virtual root node key ($nkv) then you can change
     * the whole tree, in the same way as using a selection criteria that
     * selects all records. However to do this the root node key must be
     * specified directly as the selection criteria and not included in an
     * array of keys or in a WHERE condition.
     *
     * This method differs from the base {@link TNTDbRec::Change()} method in
     * that the records position control fields can not be altered.
     *
     * @access  public
     * @param   mixed       $sel        selection criteria
     * @param   array       $set        associative array of 'fld=>val' to set
     * @param   boolean     $strict     fail unless all selected records deleted
     * @return  boolean                 true if deleted record(s) successfully
     */
    function Change($sel, $set, $strict = false) {
    
        $ret = false;

        // Start error section
        do {
            if (!$set || !is_array($set)) {
                $this->_SetError('Invalid data set');
                break;
            }

            if ($sel === $this->_nkv) {
                $sel = '*';
            }

            $num = -1;

            $where = $this->PrepareSel($sel, $num);

            if (is_null($where)) {
                break;
            }

            if ($where == '') {
                // Special case (change all records)
                $where = ($this->_tif ? 'WHERE'.substr($this->_tivSql, 3) : '');
            }

            // Remove the reserved fields from the set
            foreach ($this->_resFlds as $f) {
                if (isset($set[$f])) {
                    unset($set[$f]);
                }
            }

            $ret = parent::Change($sel, $set, $strict);

        } while (false);
        // End error section
    
        return $ret;
    }

    /**
     * Return parent record.
     *
     * If $fld is specified then the value of $fld for the parent record is
     * returned.
     *
     * If $fld is not specified then the parent record key is returned.
     *
     * If $fld is a comma separated list of field names or '*' then the
     * a structure of data for the parent record is returned.
     *
     * @access  public
     * @param   scalar      $key        key of record to get siblings for
     * @param   string      $fld        name of field to return (null for none)
     * @return  array                   array of all sibling records
     * @see     Current(), Children(), Ancestors(), Descendants(), Siblings()
     */
    function Parent($key, $fld = null) {
    
        $ret = null;

        // Start error section
        do {
            $rkv = $this->db->PrepareVal($key, $this->csf);

            // Get parent id
            $pid = $this->db->FetchValue("SELECT `{$this->_pkf}` FROM `{$this->_tab}` WHERE `{$this->_rkf}` = $rkv {$this->_tivSql}", $this->csf);
            if (is_null($pid)) {
                $this->_SetError('Can not select parent id of record: '.$key, $this->db);
                break;
            }

            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);
            $f = $flds;
            $f[] = $this->_rkf;
            $f = join(',', $this->ProcessFlds($f));

            $pkv = $this->db->PrepareVal($pid, $this->csf);

            $recs = $this->db->Fetch("SELECT $f FROM `{$this->_tab}` WHERE `{$this->_rkf}` = $pkv {$this->_tivSql}", 0, TNT_DB_FETCH_ASSOC, $this->csf);
            if (is_null($recs)) {
                $this->_SetError('Can not select record: '.$pid, $this->db);
                break;
            }
            if (count($recs) != 1) {
                $this->_SetError('No such record: '.$pid);
                break;
            }

            // Correct any depth field
            if (isset($recs[0]['_depth']) && $recs[0]['_depth'] == -1) {
                $recs[0]['_depth'] = $this->Depth($key) - 1;
            }

            $ret = $this->_ProcessRecs($recs, $flds);

            $ret = reset($ret);

        } while (false);
        /* End error section */
    
        return $ret;
    }

    /**
     * Return current record.
     *
     * If $fld is specified then the value of $fld for the current record is
     * returned.
     *
     * If $fld is not specified then the current record key is returned.
     *
     * If $fld is a comma separated list of field names or '*' then the
     * a structure of data for the current record is returned.
     *
     * This method behaves the same as {@link Exist()} if no $fld is specified,
     * or the same as {@link Obtain()} if $fld == '*'.
     *
     * @access  public
     * @param   scalar      $key        key of record to get
     * @param   string      $fld        name of field to return (null for none)
     * @return  array                   current record
     * @see     Parent(), Children(), Ancestors(), Descendants(), Siblings()
     */
    function Current($key, $fld = null) {
    
        $ret = null;

        // Start error section
        do {
            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);

            $f = $flds;
            $f[] = $this->_rkf;
            $f = join(',', $this->ProcessFlds($f));

            $rkv = $this->db->PrepareVal($key, $this->csf);

            $recs = $this->db->Fetch("SELECT $f FROM `{$this->_tab}` WHERE `{$this->_rkf}` = $rkv {$this->_tivSql}", 0, TNT_DB_FETCH_ASSOC, $this->csf);
            if (is_null($recs)) {
                $this->_SetError('Can not select record: '.$key, $this->db);
                break;
            }

            if (count($recs) != 1) {
                $this->_SetError('No such record: '.$key);
                break;
            }

            // Correct any depth field
            if (isset($recs[0]['_depth']) && $recs[0]['_depth'] == -1) {
                $recs[0]['_depth'] = $this->Depth($key);
            }

            $ret = $this->_ProcessRecs($recs, $flds);

            $ret = reset($ret);

        } while (false);
        /* End error section */
    
        return $ret;
    }

    /**
     * Return an array of all child records.
     *
     * If $fld is specified then an array is returned with the value of $fld
     * for all descendants indexed by the record key.
     *
     * If $fld is not specified then a numeric array is returned holding all the
     * descendants record keys.
     *
     * If $fld is a comma separated list of field names or '*' then the value
     * will be an structure of data. This way all the details of all
     * children can be returned.
     *
     * The order of the returned children can be specified by passing an array
     * of ordering fields (ie. array('id ASC', 'name DESC');).
     *
     * If a null sort order is specified and the node has a {$cnf} then it is
     * used to sort the records.
     *
     * @access  public
     * @param   scalar      $key        key of record to get ancestors of
     * @param   string      $fld        name of field to return (null for none)
     * @return  array       $ord        sort order
     * @see     Parent(), Current(), Ancestors(), Descendants(), Siblings()
     */
    function Children($key, $fld = null, $ord = null) {
    
        $ret = null;

        // Start error section
        do {
            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);
            $f = $flds;
            $f[] = $this->_rkf;
            $f = join(',', $this->ProcessFlds($f));

            $o = (is_null($ord) ? ($this->_cnf ? "`{$this->_cnf}` ASC" : '') : join(',', $this->ProcessOrder($ord)));
            if ($o) {
                $o = "ORDER BY $o";
            }

            $rkv = $this->db->PrepareVal($key, $this->csf);

            // Get all children of this record
            $recs =  $this->db->Fetch("SELECT $f FROM `{$this->_tab}` WHERE `{$this->_pkf}` = $rkv {$this->_tivSql} $o", 0, TNT_DB_FETCH_ASSOC, $this->csf);
            if (is_null($recs)) {
                $this->_SetError('Can not get children of record: '.$key, $this->db);
                break;
            }

            // Correct any depth field
            if (isset($recs[0]['_depth']) && $recs[0]['_depth'] == -1) {
                $depth = $this->Depth($key) + 1;
                $max_i = count($recs);
                for ($i = 0; $i < $max_i; $i++) {
                    $recs[$i]['_depth'] = $depth;
                }
            }

            $ret = $this->_ProcessRecs($recs, $flds);

        } while (false);
        /* End error section */
    
        return $ret;
    }
    
    /**
     * Return an array of all ancestor records.
     *
     * If $fld is specified then an array is returned with the value of $fld
     * for all ancestors indexed by the record key.
     *
     * If $fld is not specified then a numeric array is returned holding all the
     * ancestor record keys.
     *
     * If $fld is an array of field names or a comma separated list of field
     * names or '*' then the value will be an structure of data. This way all
     * the details of the ancestors can be returned.
     *
     * The ancestors are returned from the root down, You can specify the field
     * name '_depth' which will contain the depth of the ancestor from the root
     * (which is at depth 0).
     *
     * You can optionally include the starting node in the ancestor tree.
     *
     * @access  public
     * @param   scalar      $key        key of record to get ancestors of
     * @param   string      $fld        name of field to return (null for none)
     * @param   boolean     $inc        include starting node in results
     * @return  array                   array of all ancestor records
     * @see     Parent(), Current(), Children(), Descendants(), Siblings()
     */
    function Ancestors($key, $fld = null, $inc = false) {
    
        $ret = null;

        // Start error section
        do {
            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);

            $recs = $this->_Ancestors($key, $flds, $inc);
            if (is_null($recs)) {
                $this->_SetError('Can not get ancestors of record: '.$key, $this->db);
                break;
            }

            $ret = $this->_ProcessRecs($recs, $flds, ($inc ? null : $key));

            $recs = null; // free up this array as may be quite big
           
        } while (false);
        /* End error section */
    
        return $ret;
    }

    /**
     * Return an array of all descendant records.
     *
     * If $fld is specified then an array is returned with the value of $fld
     * for all descendants indexed by the record key.
     *
     * If $fld is not specified then a numeric array is returned holding all the
     * descendants record keys.
     *
     * If $fld is a comma separated list of field names or '*' then the value
     * will be an structure of data. This way all the details of all
     * descendants can be returned.
     *
     * The descendants are returned using a preorder traversal. You can use the
     * field name '_depth' to return a field containing the depth of the
     * descendant from the root (which is at depth 0).
     *
     * The order of the returned children can be specified by passing an array
     * of ordering fields (ie. array('id ASC', 'name DESC');).
     *
     * If a null sort order is specified and the node has a {$cnf} then it is
     * used to sort the records.
     *
     * You can optionally include the starting node in the descendants tree.
     *
     * If you specify the virtual root node key ($nkv) then you get the whole
     * tree in a more optimised fashion using a single database query.
     *
     * @access  public
     * @param   scalar      $key        key of record to get ancestors of
     * @param   string      $fld        name of field to return (null for none)
     * @param   boolean     $inc        include starting record in results
     * @return  array       $ord        sort order
     * @return  array                   array of all descendant nodes
     * @see     Parent(), Current(), Children(), Ancestors(), Siblings()
     */
    function Descendants($key, $fld = null, $inc = false, $ord = null) {
    
        $ret = null;

        // Start error section
        do {
            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);

            $recs = $this->_Descendants($key, $flds, $inc, $ord, true);
            if (is_null($recs)) {
                $this->_SetError('Can not get descendants of record: '.$key, $this->db);
                break;
            }

            $ret = $this->_ProcessRecs($recs, $flds, ($inc ? null : $key));

            $recs = null; // free up this array as may be quite big

        } while (false);
        /* End error section */
    
        return $ret;
    }
    
    /**
     * Return an array of all sibling records.
     *
     * If $fld is specified then an array is returned with the value of $fld
     * for all siblings indexed by the record key.
     *
     * If $fld is not specified then a numeric array is returned holding all the
     * sibling record keys.
     *
     * If $fld is a comma separated list of field names or '*' then the value
     * will be an structure of data. This way all the details of all
     * siblings can be returned.
     *
     * The order of the returned children can be specified by passing an array
     * of ordering fields (ie. array('id ASC', 'name DESC');).
     *
     * If a null sort order is specified and the node has a {$cnf} then it is
     * used to sort the records.
     *
     * You can optionally include the starting node in the descendants tree.
     *
     * @access  public
     * @param   scalar      $key        key of record to get siblings for
     * @param   string      $fld        name of field to return (null for none)
     * @param   boolean     $inc        include starting record in results
     * @return  array       $ord        sort order
     * @return  array                   array of all sibling records
     * @see     Parent(), Current(), Children(), Ancestors(), Descendants()
     */
    function Siblings($key, $fld = null, $inc = false, $ord = null) {
    
        $ret = null;

        // Start error section
        do {
            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);

            $recs = $this->_Siblings($key, $flds, $inc, $ord, true);
            if (is_null($recs)) {
                $this->_SetError('Can not get siblings of record: '.$key, $this->db);
                break;
            }

            $ret = $this->_ProcessRecs($recs, $flds, ($inc ? null : $key));

            $recs = null; // free up this array as may be quite big
           
        } while (false);
        /* End error section */
    
        return $ret;
    }
    
    /**
     * Export the tree branch rooted at the specified node, in a format which
     * can be easily manipluted and/or reimported.
     *
     * If you specify the virtual root node key ($nkv) then you get the whole
     * tree in a more optimised fashion using a single database query.
     *
     * The output formats are as follows;
     *  'grid'    - a two-dimensional array of records is returned. The first
     *              dimension if the parent key, and the second dimension is
     *              the record key (in positional order if supported)
     *
     *  'node'    - an array of record structures are returned where each
     *              structure contains an additional element called '_children'
     *              which in turn contains an array of child records.
     *
     *  'xml'     - an XML text representation of the tree, the records fields
     *              are given as attributes on the node. The node is named
     *              after the table name.
     *
     *  'cvs'     - an array of strings with each record in CVS format.
     *
     *  'raw'     - an array of records (this is the same as {@link Descendants()})
     *
     * @access  public
     * @param   scalar      $key        ke of record to to start branch at
     * @param   string      $fmt        format to return the tree data
     * @return  array                   array of tree data
     * @see     ImportTree()
     */
    function ExportTree($key, $fmt = 'grid') {
    
        $ret = null;

        // Start error section
        do {

            if ($fmt != 'raw') {
                $method = '_TreeExport'.$fmt;
                if (!method_exists($this, $method)) {
                    $this->_SetError('Invalid export format: '.$fmt);
                    break;
                }
            }

            // Get the tree branch rooted at $key
            $recs = $this->_Descendants($key, '*', true);
            if (is_null($recs)) {
                $this->_SetError('Can not get tree branch at node: '.$key, $this->db);
                break;
            }

            if ($fmt != 'raw') {
                $ret = $this->$method($recs, $key, $fmt);
                $recs = null; // Free array
            } else {
                $ret = $recs;
            }

        } while (false);
        /* End error section */
    
        return $ret;
    }
    
    /**
     * Import the tree branch, either to the original export position or
     * to a new position.
     *
     * NB: The imported branch is created as a new branch, that is the record
     * keys in the imported data are ignored and new records created.
     *
     * If no values are specified for the new position ($key, $ord) then the
     * branch is imported to the parent and position it was exported from as
     * given in the parent record key and position of the branch root.
     *
     * If value are given for the new position then the branch root node is
     * created in the specified position (refer to {@link Create()}).
     *
     * It is intended that the recieving table is the same one that the data
     * was exported from. However they need not be as the table name is not
     * used in the imported data - however the record key field, parent key
     * field would need to have the same name. If the recieving table has no
     * positioning field then non will be used from the inported data.
     *
     * For a list of import formats refer to {@link ExportTree()}.
     *
     * @access  public
     * @param   mixed       $dat        dat to import (as given from Export())
     * @param   string      $fmt        format the data is in
     * @param   array       $pos        records position in hierachy (or null)
     * @return  array                   array of tree data
     * @see     ExportTree()
     */
    function ImportTree($data, $fmt, $pos = null) {
    
        $ret = null;

        // Start error section
        do {
            if (!$data) {
                $this->_SetError('No data to import');
                break;
            }

            if ($fmt != 'raw') {
                $method = '_TreeImport'.$fmt;
                if (!method_exists($this, $method)) {
                    $this->_SetError('Invalid import format: '.$fmt);
                    break;
                }
            }

            // Validate the position 
            if ($pos && !$this->ChkPos($pos)) {
                break;
            }

            // Convert the data to a raw array of records to create
            if ($fmt != 'raw') {
                $recs = $this->$method($data, $fmt, $pos);
                if (is_null($recs)) {
                    break;
                }
            } else {
                $recs = $data;
                if (!is_array($recs)) {
                    $this->_SetError('Invalid data: '.$fmt);
                    break;
                }
            }

            if (!($max_i = count($recs))) {
                $ret = true;
                break;
            }

            $ret = $this->CreateTree($recs, $pos);

        } while (false);
        /* End error section */
    
        return $ret;
    }
    
    /**
     * Returns the number of immediate children of a record.
     *
     * Results of this method are cached for speed. Other methods will update
     * this cache as required.
     * 
     * @access  public
     * @param   scalar      $key        key of record to count children of
     * @return  integer                 number of children (or null)
     */
    function ChildCount($key) {

        $ret = null;

        // Start error section
        do {
            if (isset($this->__childCount[$key])) {
                $ret = $this->__childCount[$key];
                break;
            }

            $pkv = $this->db->PrepareVal($key, $this->csf);

            $recs = $this->db->Fetch("SELECT COUNT(*) FROM `{$this->_tab}` WHERE `{$this->_pkf}` = $pkv {$this->_tivSql}", 0, TNT_DB_FETCH_NUM, false);
            if (is_null($recs)) {
                $this->_SetError('Can not get number of children of record: '.$key, $this->db);
                break;
            }

            $ret = (count($recs) ? (int)($recs[0][0]) : 0);

            $this->__childCount[$key] = $ret;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Returns the depth of a record from the root.
     *
     * If the record has a dedicated depth field (set using the 'ndf' option
     * to the constructor) then the method will use that to get the records
     * depth.
     *
     * If no depth field is given then the ancestor chain will be obtained to
     * determine the depth.
     *
     * If you ask for the depth of the virtual root node then the method will
     * return -1. This allows your depth to be always 1 more than your parents
     * even if your parent is the virtual root node.
     *
     * Results of this method are cached for speed. Other methods will update
     * this cache as required.
     * 
     * @access  public
     * @param   scalar      $key        key of record to get depth of
     * @return  integer                 depth of record (or null)
     */
    function Depth($key) {

        $ret = null;

        // Start error section
        do {
            if ($key == $this->_nkv) {
                $ret = -1;
                break;
            }

            if (isset($this->__depth[$key])) {
                $ret = $this->__depth[$key];
                break;
            }

            if ($this->_ndf) {
                $rkv = $this->db->PrepareVal($key, $this->csf);

                $ret = $this->db->FetchValue("SELECT `{$this->_ndf}` FROM `{$this->_tab}` WHERE `{$this->_rkf}` = $rkv {$this->_tivSql}", 0, TNT_DB_FETCH_NUM, false);
                if (is_null($ret)) {
                    $this->_SetError('Can not get depth of record: '.$key, $this->db);
                    break;
                }
            } else {
                $recs = $this->_Ancestors($key);
                if (is_null($recs)) {
                    $this->_SetError('Can not get ancestor chain to calculate depth of record: '.$key, $this->db);
                    break;
                }

                $ret = count($recs);

                t_dump("Depth of record '$key' is '$ret'", $recs);

                $recs = null;
            }

            $this->__depth[$key] = $ret;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Returns the number of descendants of a record.
     *
     * Results of this method are cached for speed. Other methods will update
     * this cache as required.
     * 
     * @access  public
     * @param   scalar      $key        key of record to count children of
     * @return  integer                 number of descendants (or null)
     */
    function DescendantCount($key) {

        $ret = null;

        // Start error section
        do {
            if (isset($this->__descendantCount[$key])) {
                $ret = $this->__descendantCount[$key];
                break;
            }

            $recs = $this->Descendants($key);
            if (is_null($recs)) {
                break;
            }

            $ret = count($recs);

            $this->__descendantCount[$key] = $ret;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Find an absolute position given a relative positon.
     *
     * Finds the absolute position for a record given a relative position
     * to some other record.
     *
     * Three arguments are required;
     *      $key - key of record that position is relative to
     *      $ord - position 0 = first, etc.
     *      $rel - position as child of $key, sibling of $key or
     *             relative sibling of $key
     *
     * Eg.
     *      (1, 4, TNT_DBNODE_POS_CHILD)    - 5th child of record 1.
     *      (2, 0, TNT_DBNODE_POS_CHILD)    - 1st child of record 2.
     *      (3, 0, TNT_DBNODE_POS_SIBLING)  - 1st child of parent of record 1.
     *      (4, 1, TNT_DBNODE_POS_SIBLING)  - 2nd child of parent of record 2.
     *      (5, 1, TNT_DBNODE_POS_SIBLING_REL) - next to record 5.
     *      (5, 3, TNT_DBNODE_POS_SIBLING_REL) - next but 2 to record 5.
     *      (6, 0, TNT_DBNODE_POS_SIBLING_REL) - prev to record 6.
     *      (6, -2, TNT_DBNODE_POS_SIBLING_REL) - prev but 2 from record 6.
     *
     * Positions can be given absolute for CHILD and SIBLING or relative for
     * SIBLING_REL. Relative positions are offsets from the position of the
     * specified child. For example, (6, -2, SIBLING_REL) will give a position
     * of 10 if the position of record 6 is 12. A relative position of 0
     * means just before the record and 1 means just after it.
     *
     * The returned position will be between 0 and the (number of children + 1),
     * positions outside this range will be coerced to it.
     *
     * To specify the last child/sibling use the value TNT_DBNODE_POS_MAX.
     *
     * The '$key' argument can be given as a scalar record key, in which case
     * the record will be loaded to determine its position etc., or it can be
     * given as a structure containing an already loaded record.
     *
     * The method will return an structure as follows;
     *  'pid'   - parent key of record
     *  'num'   - child number of record (0 = first) [if supported]
     *
     * If positioning is not supported then this method will return the parent
     * key of the record only.
     *
     * To use child numbering a field must exist in the record to store
     * the child number value. The name of this field must be passed to
     * the contructor as the option 'cnf'.
     * 
     * @access  public
     * @param   mixed       $key        key to record to make space relative to
     * @param   integer     $num        records child number
     * @param   const       $rel        relation to specified record
     * @return  array                   'pid' and 'num' of space allocated
     */
    function AbsPos($key, $num, $rel) {

        $ret = null;

        // Start error section
        do {

            if (is_array($key)) {
                $rec = $key;
                $key = $rec[$this->_rkf];
            } else {
                $rec = null;
            }

            switch ($rel) {
                case 'TNT_DBNODE_POS_SIBLING':
                case 'TNT_DBNODE_POS_SIBLING_REL':
                    // Position as child of $key
                    if (!is_array($rec)) {
                        // Get the parent key and order of this child
                        $rkv = $this->db->PrepareVal($key, $this->csf);
                        $cnf = ($this->_cnf ? ", `{$this->_cnf}`" : '');
                        $recs = $this->db->Fetch("SELECT `{$this->_pkf}` $cnf FROM `{$this->_tab}` WHERE `{$this->_rkf}` = $rkv {$this->_tivSql}", 0, TNT_DB_FETCH_ASSOC, $this->csf);
                        if (is_null($recs)) {
                            $this->_SetError('Can not get details of child: '.$key, $this->db);
                            break;
                        }
                        if (count($recs) != 1) {
                            $this->_SetError('No such child record: '.$key);
                            break;
                        }

                        $rec = $recs[0];
                    }

                    $key = $rec[$this->_pkf];

                    if ($this->_cnf && $rel == TNT_DBNODE_POS_SIBLING_REL) {
                        $num = ($rec[$this->_cnf] + $num);
                    }

                    // Drop through

                case 'TNT_DBNODE_POS_CHILD':
                    // Position as child of $key

                    if (!$this->_cnf) {
                        $ret = new TNTDbNodePos($key, null);
                        break;
                    }

                    // Get number of children
                    $max = $this->ChildCount($key);
                    if (is_null($max)) {
                        break;
                    }

                    if ($num < 0) {
                        $cnv = 0;
                    } else if ($num > $max + 1) {
                        $cnv = $max + 1;
                    } else {
                        $cnv = (int)$num;
                    }

                    $ret = new TNTDbNodePos($key, $cnv);

                    break;

                default:
                    $this->_SetError('Invalid position: '.$num);
                    break;
            }

        } while (false);
        // End error section
    
        return $ret;
    }

    /**
     * Validate and normalize a position.
     *
     * This method will validate the parent key of a position stucture exists
     * and the normalize the child number to be within the child count of the
     * child (+1 allowing for appending a new child).
     *
     * @access  public
     * @param   array       $pos        records position in hierachy (or null)
     * @return  boolean                 true if parent exists
     */
    function ChkPos(&$pos) {

        $ret = false;

        // Start error section
        do {

            if (!is_a($pos, 'TNTDbNodePos')) {
                $this->_SetError('Invalid position structure');
                break;
            }

            if (!$this->_cnf) {
                $pos->cnv = null;

                if ($pos->pkv != $this->_nkv && !$this->Exists($pos->pkv)) {
                    $this->_SetError('Parent record does not exist: '.$pos->pkv);
                    break;
                }
                
                $ret = true;

                break;
            }

            // Get number of children this parent has
            $max = $this->ChildCount($pos->pkv);
            if (is_null($max)) {
                break;
            }

            // Normalize the number
            if ($pos->cnv < 0) {
                $pos->cnv = 0;
            } else if ($pos->cnv > $max + 1) {
                $pos->cnv = $max + 1;
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }

    /**
     * Get the position of a record in the tree.
     *
     * The method will return an structure as follows;
     *  'pid'   - parent key of record
     *  'num'   - child number of record (0 = first) [if supported]
     *
     * The results of this method are cached. The cache is updated by a call
     * to {@link SetPos()} or {@link Create()}.
     *
     * To use child numbering a field must exist in the record to store
     * the child number value. The name of this field must be passed to
     * the contructor as the option 'cnf'.
     *
     *
     * @access  public
     * @param   scalar      $key        key to record to get position of
     * @return  array                   records position in hierachy (or null)
     */
    function GetPos($key) {
    
        $ret = null;

        // Start error section
        do {
            // Check cached value
            if (isset($this->_getpos[$key])) {
                $ret = $this->_getpos[$key];
                break;
            }

            $rkv = $this->db->PrepareVal($key, $this->csf);

            $f = "`{$this->_pkf}`";
            if ($this->_cnf) {
                $f .= ",`{$this->_cnf}`";
            }

            $recs = $this->db->Fetch("SELECT $f FROM `{$this->_tab}` WHERE `{$this->_rkf}` = $rkv {$this->_tivSql}");
            if (is_null($recs)) {
                $this->_SetError('Can not select record: '.$key, $this->db);
                break;
            }
            if (count($recs) != 1) {
                $this->_SetError('No such record: '.$key);
                break;
            }

            $ret = new TNTDbNodePos($recs[0][$this->_pkf], ($this->_cnf ? $recs[0][$this->_cnf] : null));

            $this->_getpos[$key] = $ret;

        } while (false);
        // End error section
    
        return $ret;
    }

    /**
     * Set the position of a record in the tree.
     *
     * This can be used to move nodes about the tree.
     *
     * Repositiong a record will update any modification timestamp if enabled.
     *
     * @access  public
     * @param   scalar      $key        key to record to move
     * @param   array       $pos        records position in hierachy (or null)
     * @return  boolean                 true if set new position successfully
     */
    function SetPos($key, $pos) {
    
        $ret = false;

        // Start error section
        do {

            // Get records current position
            $old_pos = $this->GetPos($key);
            if (is_null($old_pos)) {
                break;
            }

            if ($old_pos->pkv == $pos->pkv && 
                $old_pos->cnv == $pos->cnv) {
                // no movement
                $ret = true;
                break;
            }

            // Validate that the new position is ok
            if (!$this->ChkPos($pos)) {
                break;
            }

            // Make space at the given position for new child
            if (!$this->_MakeSpace($pos)) {
                break;
            }

            $rkv = $this->db->PrepareVal($key, $this->csf);

            $pkv = $this->db->PrepareVal($pos->pkv, $this->csf);
            $set = "`{$this->_pkf}` = $pkv";

            if ($this->_cnf) {
                $cnv = $this->db->PrepareVal($pos->cnv, $this->csf);
                $set = ", `{$this->_cnf}` = $cnv";
            }

            if ($this->uts && $this->hts) {
                $set = ', `mtime` = '.time();
            }

            // Update record into new location
            if (!$this->db->Query("UPDATE `{$this->_tab}` SET $set WHERE `{$this->_rkf}` = $rkv {$this->_tivSql}")) {
                $this->_SetError('Can not update record: '.$key, $this->db);
                break;
            }

            // Close up the ordering in the old location
            if ($this->_cnf) {
                if (!$this->db->Query("UPDATE `{$this->_tab}` SET `{$this->_cnf}` = `{$this->_cnf}` - 1 WHERE `{$this->_cnf}` >= $cnv AND `{$this->_pkf}` = $pkv {$this->_tivSql}")) {
                    $this->_SetError('Can not re-number children of node: '.$pos->pkv, $this->db);
                    break;
                }
                if (isset($this->_childCount[$pos->pkv])) {
                    $this->_childCount[$pos->pkv]++;
                }
                if (isset($this->_descendantCount[$pos->pkv])) {
                    $this->_descendantCount[$pos->pkv]++;
                }
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }

    /**
     * Returns the key of a record given its path.
     * 
     * The path to a record is an array of values of some secondary field at
     * each level in the hierachy. The secondary field must be unique with in
     * its parent, that is ($pkf,$fld) must be a unqiue key, however it need
     * not be defined as a key in the database schema.
     *
     * If no field is specified then the record key field is used.
     *
     * If no path is specified then value of root node is returned.
     *
     * If $path is passed as a string value then it is assumed to be a
     * string of key values separated by a '/' character.
     *
     * @access  public
     * @param   scalar[]    $path       array of key values
     * @param   string      $keyFld     name of key field to use.
     * @return  scalar                  value of key of record (or null)
     */
    function PathToKey($path, $fld = null) {

        $ret = false;

        /* Start error section */
        do {

            if (is_null($fld)) {
                $fld = $this->_rkv;
            }

            $key = $this->_nkv;

            if (!$path) {
                $ret = $key;
                break;
            }
            
            if (is_string($path)) {
                $path = explode('/', trim($path, '/'));
            }

            foreach ($path as $val) {

                $v = $this->db->PrepareVal($val, $this->csf);
                $pkv = $this->db->PrepareVal($key, $this->csf);

                $key = $this->db->FetchValue("SELECT `{$this->_rkf}` FROM `{$this->_tab}` WHERE `{$this->_pkf}` = $pkv {$this->_tivSql} AND `$fld`=$v", $this->csf);
                if (is_null($key)) {
                    $this->_SetError("Can not select a record where `{$this->_pkf}` = $pkv and `$fld` = $v", $this->db);
                    $key = null;
                    break;
                }
            }

            $ret = $key;

        } while (false);
        /* End error section */

        return $ret;
    }
    
    /**
     * Returns the path of a record given its key.
     * 
     * The path to a record is an array of values of some secondary field at
     * each level in the hierachy. The secondary field must be unique with in
     * its parent, that is ($pkf,$fld) must be a unqiue key, however it need
     * not be defined as a key in the database schema.
     *
     * This is equivalent to 'Ancestors($key, $fld, true)'.
     *
     * If no field is specified then the record key field is used.
     *
     * @access  public
     * @param   scalar      $key        key of record to get path for
     * @param   string      $keyFld     name of key field to use.
     * @return  mixed                   value of key of record (or null)
     */
    function KeyToPath($key, $fld = null) {

        if (is_null($fld)) {
            $fld = $this->_rkv;
        }

        return $this->Ancestors($key, $fld, true);
    }
    
    /**
     * Check the object is valid for use.
     *
     * Checks that a valid database is available and the table and fields
     * are known.
     *
     * @access  public
     * @return  boolean                 true if object is valid
     */
    function IsValid() {
        if (!parent::IsValid()) {
            $ret = false;
        } else if (!$this->_pkf) {
            $this->_SetError('No valid parent foriegn key field');
            $ret = false;
        }
        return $ret;
    }

    /**
     * Get the tree identifier.
     *
     * The Tree Indentifier is saved in each node and used to identify which
     * nodes belong to which tree when fetching or modifying nodes on mass.
     *
     * To use a Tree Identifier a field must exist in the record to store
     * the identification value. The name of this field must be passed to
     * the contructor as the option 'tif'.
     *
     * @access  public
     * @return  scalar                  tree identification value
     */
    function GetTreeId() {
    
        if ($this->_tif) {
            $ret = $this->_tiv;
        } else {
            $this->_SetError('Tree identification not supported');
            $ret = null;
        }

        return $ret;
    }

    /**
     * Set the Tree Identification.
     *
     * The Tree Indentifier is saved in each node and used to identify which
     * nodes belong to which tree when fetching or modifying nodes on mass.
     *
     * To use a Tree Identifier a field must exist in the record to store
     * the identification value. The name of this field must be passed to
     * the contructor as the option 'tif'.
     *
     * @access  public
     * @param   scalar      $id         new tree identification value
     * @return  boolean                 true if successfull
     */
    function SetTreeId($id) {

        if (!$this->_tif) {
            $this->_SetError('Tree identification not supported');
            $ret = false;
        } else if (is_null($id)) {
            $this->_SetError('Tree identification can not be null');
            $ret = false;
        } else {
            $this->_tiv = $id;
            $this->_tivSql = "AND `{$this->_tif}` = ".$this->db->PrepareVal($this->_tiv, $this->csf);
            $ret = true;
        }
    
        return $ret;
    }

    /**
     * Construct the object
     *
     * Supported construction options are;
     *   'db' - reference to database instance to use
     *   'uts' - boolean to indicate timestamps should be updated
     *   'csf' - boolean to indicate compound subfields should be used
     *   'nkv' - value used as the parent id of the root nodes (default 0)
     *   'tif' - tree identifier field name
     *   'tiv' - tree identifier field value
     *   'ndf' - node depth field name
     *   'cnf' - child number field name
     *
     * If no record key is specified then the schema is checked for the name
     * of the primary key field and it is used.
     *
     * The parent key must be specified, if not then the value 'pid' is used.
     *
     * The child number field is option and if not specified it will not be
     * used and no child ordering will be supported.
     *
     * @access  public
     * @param   string     $table       pseudo database table name
     * @param   string     $rkf         record key field name in table
     * @param   string     $pkf         parent foriegn key field name in table
     * @param   array      $options     construction options (optional)
     * @return  TNTDbNode&              reference to a new object
     */
    function TNTDbNode($table, $rkf = null, $pkf = null, $options = null) {

        TNTDbRec::TNTDbRec($table, $rkf, $options);

        $this->_pkf = ($pkf ? $pkf : 'pid');

        $this->_tif = (isset($options['tif']) ? $options['tif'] : null);
        $this->_ndf = (isset($options['ndf']) ? $options['ndf'] : null);
        $this->_cnf = (isset($options['cnf']) ? $options['cnf'] : null);
        $this->_nkv = (isset($options['nkv']) ? $options['nkv'] : 0);

        if ($this->_tif) {
            $this->SetTreeId(isset($options['tiv']) ? $options['tiv'] : 0);
        }

        $this->_xFlds = array('_depth');
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTDbNode() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
