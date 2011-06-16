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
 * Database Materialized Path Node Access Class
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class is a extension to the basic {@link TNTDbNode} class to provide
 * support for materialized paths.
 *
 * [Refer to http://www.dbazine.com/oracle/or-articles/tropashko4]
 *
 * @package TNT.Database
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTDbNode');

/*
 * Globals
 */

/*
 * Defines
 */


/**
 * Database Materialized Path Node Access Class
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class is a extension to the basic {@link TNTDbNode} class to provide
 * support for materialized paths.
 *
 * 'Materialized Path' type trees are more efficient than the simple
 * 'Adjacency List Model' used by the {@link TNTDbNode} class. This is because
 * the actual path to the node is stored in the nodes data. This not only means
 * that the path mapping methods {@link PathToKey()} and {@link KeyToPath()}
 * will be more efficient when used on the materialized path, but the hierachy
 * methods can work using single database queries selecting on the materialized
 * path field.
 *
 * This class supports the idea of multiple materialized paths, although only
 * one is needed for the hierachy methods, other can just be materialized if
 * the path mapping methods are commonly used on different paths.
 *
 * For example if the record has a field called 'name' which is used to make
 * a path of the format '<name>/<name>' etc. then a materialized path mapping
 * of 'name' => 'path' can be given to the class which will cause it to maintain
 * this path in a field called 'path' in the records data.
 *
 *
 * @package TNT.Database
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTDbMpNode extends TNTDbNode {

    /**
     * Array of materialized path mappings.
     * @var     string[]                database column name mappings
     * @access  private
     */
    var $_mpm = null;

    /**
     * Set if the paths are inconsistant and need rebuilding
     * @var     boolean                 true if need to {@link RebuildPaths()}
     * @access  private
     */
    var $_doRebuildPaths = false;

    /**
     * Create record cache (used when creating trees)
     * @var     array                   array of records
     * @access  private
     */
    var $_recCache = array();

    /**
     * Flag to indicate not to check for duplicate values in path
     * @var     boolean                 true if no duplicates check
     * @access  private
     */
    var $_noDuplicateCheck = true;

    /**
     * Translate a field.
     *
     * An optional table name can be prefixed to the fields.
     *
     * The field name must be one of the tables fields or one of the special
     * field names as follows;
     *   + '_depth' - returns the depth of the node (or -1 if unknown)
     *   + '_path'  - returns the absolute path to the node (or '' if unknown)
     *
     * Any other fields starting with '_' are passed to the base method and
     * return as null if unkown and should be ignored.
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
        if ($fld == '_depth' && !$this->_ndf) {
            reset($this->_mpm);
            list($mpm_f, $mpm_p) = each($this->_mpm);
            $p = parent::_TranslateFld($mpm_p, $table);
            $ret = "(LENGTH($p) - LENGTH(REPLACE($p, '/', ''))) - 1 AS _depth";

        } else if ($fld == '_path') {
            reset($this->_mpm);
            list($mpm_f, $mpm_p) = each($this->_mpm);
            $p = parent::_TranslateFld($mpm_p, $table);
            $f = parent::_TranslateFld($mpm_f, $table);
            $ret = "CONCAT($p, REPLACE($f, '/', '_'), '/') AS _path";
        } else {
            $ret = parent::_TranslateFld($fld, $table);
        }
        return $ret;
    }

    /**
     * Fix the paths.
     *
     * Update all paths stored in $fld so that paths starting with $old are
     * modified to start with $new.
     *
     * @access  public
     * @param   string      $fld        path field name
     * @param   string      $old        old path
     * @param   string      $new        new path
     * @return  boolean                 true if successfull
     */
    function _FixPaths($fld, $old, $new) {

        $ret = null;

        // Start error section
        do {
            if ($old == $new) {
                $ret = true;
                break;
            }

            // All occurances of $old... change to $new...
            $_new = $this->db->EscapeString($new, false);
            $_old = $this->db->EscapeString($old, false);

            // Escape slashes and quote characters
            $_old = addcslashes($_old, '\\_%');

            $_len = strlen($old);

            if (!$this->db->Query("UPDATE {$this->_tab} SET `{$fld}` = INSERT(`{$fld}`, 1, {$_len}, '{$new}') WHERE `{$fld}` LIKE '{$old}%'")) {
                $this->_SetError("Can not update paths '{$old}...' to '{$new}...'", $this->db->_GetError());
                break;
            }

            $ret = true;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Rebuild all the paths.
     *
     * This will go through the whole tree rebuilding the paths.
     *
     * @access  public
     * @return  boolean                 true if successfull
     */
    function _RebuildPaths() {
        $ret = false;

        // Start error section
        do {

            $ret = true;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Check unique values are used for the path elements for each level
     * in the hierachy.
     *
     * @access  public
     * @param   array       $set        record changes
     * @param   string      $where      recrod selection criteria
     * @return  boolean                 true if successfull
     */
    function _CheckUnique($set, $where) {
        $ret = false;

        // Start error section
        do {

            // Check that all the maintained path field are unique
            $u = null;
            foreach ($set as $f => $v) {
                if (isset($this->_mpm[$f])) {
                    $u .= ($u ? ' OR ' : '')."`$f` = ".$this->db->PrepareVal($v, $this->csf);
                }
            }

            if (!$u) {
                // Changed effect no paths
                $ret = true;
                break;
            }

            if ($where == '') {
                // Special case (change all records)
                $w = "($u) {$this->_tivSql}";
            } else {
                $w = "(($u) AND (".substr($where, 6).")) {$this->_tivSql}";
            }

            // Get parent id all records which would get a duplicate value
            $recs = $this->db->Fetch("SELECT COUNT(`{$this->_pkf}`) AS _count, `{$this->_pkf}` FROM `{$this->_tab}` WHERE $w GROUP BY `{$this->_pkf}` HAVING _count > 1", 0, TNT_DB_FETCH_NUM, false);

            if (is_null($recs)) {
                $this->_SetError('Can not check for duplicate path elements: WHERE '.$where, $this->db->_GetError());
                break;
            }

            if (count($recs) > 0) {
                $this->_SetError('Duplicate path element field detected, the values of ('.join(',', array_keys($this->_mpm)).') must be unique with in the parent record');
                break;
            }

            $ret = true;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Maintain the paths when the specified records are changed.
     *
     * We check to see if any of the fields that go to make the materialized
     * are included in the fields being altered. If there are we work out the
     * new and old paths and replace these accross the whole tree.
     *
     * @access  public
     * @param   string      $where      record selection critera
     * @param   array       $set        record changes
     * @return  boolean                 true if successfull
     */
    function _MaintainPaths($set, $where) {
        $ret = false;

        // Start error section
        do {

            // Set if any of the fields to be set are path elements
            $mpm = array();
            foreach ($set as $fld => $val) {
                if (isset($this->_mpm[$fld])) {
                    $mpm[$fld] = $this->_mpm[$fld];
                }
            }

            if (!$mpm) {
                // Changed effect no paths
                $ret = true;
                break;
            }

            if ($where == '') {
                // Special case (change all records)
                $w = ($this->_tif ? 'WHERE'.substr($this->_tivSql, 3) : '');
            } else {
                $w = "$where {$this->_tivSql}";
            }

            // Get the old records
            $recs = $this->db->Fetch("SELECT * FROM {$this->_tab} $w", 0, TNT_DB_FETCH_ASSOC, $this->csf);

            if (is_null($recs)) {
                $this->_SetError('Can not get changed record: '.$where, $this->db->_GetError());
                break;
            }

            if (!($max_i = count($recs))) {
                $this->_SetError('No record are to be changed: '.$where);
                break;
            }

            $ok = true;
            // Update the paths on the records one by one
            for ($i = 0; $ok && $i < $max_i; $i++) {
                $rec =& $recs[$i];
                foreach ($mpm as $f => $p) {
                    if (isset($set[$f]) && $rec[$f] != $set[$f]) {
                        // If the new value is not same as old one then
                        $old = $rec[$p].str_replace('/', '_', $rec[$f]).'/';
                        $new = $rec[$p].str_replace('/', '_', $set[$f]).'/';
                        if (!($ok = $this->_FixPaths($p, $old, $new))) {
                            break;
                        }
                    }
                }
            }
            if (!$ok) {
                break;
            }

            $ret = true;

        } while (false);
        // End error section

        $this->_doRebuildPaths = !$ret;

        return $ret;
    }

    /**
     * Called just before and after a record is to be created.
     *
     * This is overridden from the base method to trigger the path
     * maintainence methods.
     *
     * @access  private
     * @param   boolean     $pre        true if called before SQL INSERT
     * @param   array       $rec        record being created
     * @return  boolean                 true if successfull
     */
    function _OnCreate($pre, &$rec) {

        $ret = false;

        // Start error section
        do {
            // If the paths need rebuilding do that before we go any further
            if ($this->_doRebuildPaths && !$this->_RebuildPaths()) {
                break;
            }
                
            if (!parent::_OnCreate($pre, $rec)) {
                break;
            }

            if (!$this->_mpm) {
                // No paths to maintain
                $ret = true;
                break;
            }

            if (!$pre) {
                if ($this->_recCache) {
                    // Cache the record if we are creating a tree
                    $this->_recCache[$rec[$this->_rkf]] = $rec;
                }
                $ret = true;
                break;
            }

            if (!$this->_noDuplicateCheck) {
                $pkv = $this->db->PrepareVal($rec[$this->_pkf]);

                $where =  "WHERE (`{$this->_pkf}` = $pkv) {$this->_tivSql}";

                // Check that all the maintained path field are unique
                if (!$this->_CheckUnique($rec, $where)) {
                    break;
                }
            }

            if ($rec[$this->_pkf] != $this->_nkv) {
                // Append field value to record paths
                if (isset($this->_recCache[$rec[$this->_pkf]])) {
                    $prec = $this->_recCache[$rec[$this->_pkf]];
                } else {
                    $pkv = $this->db->PrepareVal($rec[$this->_pkf]);
                    $recs = $this->db->Fetch("SELECT * FROM {$this->_tab} WHERE `{$this->_rkf}` = $pkv {$this->_tivSql}", 0, TNT_DB_FETCH_ASSOC, $this->csf);
    
                    if (is_null($recs) || count($recs) != 1) {
                        $this->_SetError('Can not get parent record: '.$rec[$this->_pkf], $this->db->_GetError());
                        break;
                    }
    
                    $prec = $recs[0];

                    $this->_recCache[$prec[$this->_rkf]] = $prec;;
                }

                foreach ($this->_mpm as $f => $p) {
                    $rec[$p] = $prec[$p].str_replace('/', '_', $prec[$f]).'/';
                }
            } else {
                // Initialise records paths with field values
                foreach ($this->_mpm as $f => $p) {
                    $rec[$p] = '/';
                }
            }

            $ret = true;

        } while (false);
        // End error section

        $this->_doRebuildPaths = !$ret;

        return $ret;
    }

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
     * @param   array       $recs       array of records being created
     * @param   TNTDbNodePos $pos       position records are being created in
     * @return  boolean                 true if successfull
     */
    function _OnCreateTree($pre, &$recs, &$pos) {

        $ret = false;

        // Start error section
        do {
            // If the paths need rebuilding do that before we go any further
            if ($this->_doRebuildPaths && !$this->_RebuildPaths()) {
                break;
            }
                
            if (!parent::_OnCreateTree($pre, $recs, $pos)) {
                break;
            }

            if (!$this->_mpm) {
                // No paths to maintain
                $ret = true;
                break;
            }

            // Clear the record cache
            $this->_recCache = array();

            if ($pre) {
                // Before starting we have to perform a duplicates check on
                // nodes, this is faster than doing it in _OnCreate() as that
                // does a database search at each level while we only need
                // do a database search at the root level and the a recs search
                // at other levels.

                // The records should be in preorder tranversal, that is the
                // root comes first and then each parent preceeds its children
                // The record and parent record key fields are initialised to
                // dummy integer values using the correct relationships.
                //
                // There may be multiple root records, however the first record
                // must always be a root record.
                $rootid = $recs[0][$this->_pkf];

                $ok = true;

                $recs2 = array();

                $max_i = count($recs);

                // Loop around checking each record and collect via parent
                for ($i = 0; $i < $max_i; $i++) {
                    if (!isset($recs[$i][$this->_rkf]) ||
                        !isset($recs[$i][$this->_pkf])) {
                        $this->_SetError('Invalid record: '.var_info($recs[$i]));
                        $ok = false;
                        break;
                    }
                    $recs2[$recs[$i][$this->_pkf]][$recs[$i][$this->_rkf]] =& $recs[$i];
                }
                if (!$ok) {
                    break;
                }

                // Loop again performing a duplicates check
                for ($i = 0; $i < $max_i; $i++) {
                    $rec =& $recs[$i];

                    $id = $rec[$this->_rkf];
                    $pid = $rec[$this->_pkf];

                    // have to check each record with same parent
                    foreach ($recs2[$pid] as $_id => $_rec) {
                        if ($_id == $id) {
                            continue;
                        }
                        foreach ($this->_mpm as $f => $p) {
                            if ($_rec[$f] == $rec[$f]) {
                                $this->_SetError('Duplicate path element field detected, the values of ('.join(',', array_keys($this->_mpm)).') must be unique with in parent record: '.$pid);
                                $ok = false;
                                break;
                            }
                        }
                        if (!$ok) {
                            break;
                        }
                    }
                }
                if (!$ok) {
                    break;
                }

                // If this is a root node we also have to check it against
                // existing nodes
                if ($pid == $rootid) {
                    // Simple use database to check against new parent node
                    $pkv = $this->db->PrepareVal($pos ? $pos->pkv : $pid);
    
                    $where =  "WHERE (`{$this->_pkf}` = $pkv) {$this->_tivSql}";
    
                    // Check that all the maintained path field are unique
                    if (!$this->_CheckUnique($rec, $where)) {
                        break;
                    }
                }

                $this->_noDuplicateCheck = true;
            } else {
                // Switch back on the duplicate check in _OnCreate()
                $this->_noDuplicateCheck = false;
            }

            $ret = true;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Called just before and after a record is to be modified.
     *
     * If the method returns false then the triggering method will fail. This
     * method should call {@link _SetError()} to indicate why it failed.
     *
     * If the after event is failed then the update will have been done so it
     * is up to this method to reverse the update before returning failed.
     *
     * @access  private
     * @param   boolean     $pre        true if called before SQL UPDATE
     * @param   array       $rec        record being modified
     * @return  boolean                 true if successfull
     */
    function _OnModify($pre, $rec) {

        $ret = false;

        // Start error section
        do {
            // If the paths need rebuilding do that before we go any further
            if ($this->_doRebuildPaths && !$this->_RebuildPaths()) {
                break;
            }

            $rkv = $this->db->PrepareVal($rec[$this->_rkf]);

            $where =  "WHERE (`{$this->_rkf}` = $rkv) {$this->_tivSql}";

            // Check that all the maintained path field are unique
            if (!$this->_CheckUnique($rec, $where)) {
                break;
            }

            if (!parent::_OnModify($pre, $rec)) {
                break;
            }

            if (!$pre || !$this->_mpm) {
               $ret = true;
               break;
            }

            $ret = $this->_MaintainPaths($rec, $where);

        } while (false);
        // End error section

        $this->_doRebuildPaths = !$ret;

        return $ret;
    }

    /**
     * Called just before and after records are to be changed.
     *
     * If the method returns false then the triggering method will fail. This
     * method should call {@link _SetError()} to indicate why it failed.
     *
     * If the after event is failed then the update will have been done so it
     * is up to this method to reverse the update before returning failed.
     *
     * This method can be rather slow as it has to load up ever record that
     * is being changed and change its paths. However it is not envisaged that
     * global changes like this would be made to fields that form path of the
     * records path as such fields have to be unique to the tree level they
     * are contained in.
     *
     * @access  private
     * @param   boolean     $pre        true if called before SQL UPDATE
     * @param   array       $set        array of updates to perform
     * @param   string      $where      SQL where condition for records effected
     * @return  boolean                 true if successfull
     */
    function _OnChange($pre, $set, $where) {

        $ret = false;

        // Start error section
        do {
            // If the paths need rebuilding do that before we go any further
            if ($this->_doRebuildPaths && !$this->_RebuildPaths()) {
                break;
            }

            if (!parent::_OnChange($pre, $rec)) {
                break;
            }

            // Check that all the maintained path field are unique
            if (!$this->_CheckUnique($rec, $where)) {
                break;
            }

            if (!$pre || !$this->_mpm) {
               $ret = true;
               break;
            }

            $ret = $this->_MaintainPaths($set, $where);

        } while (false);
        // End error section

        $this->_doRebuildPaths = !$ret;

        return $ret;
    }

    /**
     * Called just before and after records are to be deleted.
     *
     * If the method returns false then the triggering method will fail. This
     * method should call {@link _SetError()} to indicate why it failed.
     *
     * If the after event is failed then the delete will have been done so it
     * is up to this method to reverse the delete before returning failed.
     *
     * @access  private
     * @param   boolean     $pre        true if called before SQL DELETE
     * @param   string      $where      SQL where condition for records effected
     * @return  boolean                 true if successfull
     */
    function _OnDelete($pre, $where) {
        // If the paths need rebuilding do that before we go any further
        if ($this->_doRebuildPaths && !$this->_RebuildPaths()) {
            return false;
        }
        return parent::_OnDelete($pre, $where);
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
     * the specified ancestor. The records key field is always returned.
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
     * The ancestors are returned from the root down, You can specify the field
     * name '_depth' which will contain the depth of the ancestor from the root
     * (which is at depth 0).
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
            if (!$this->_mpm) {
                $ret = parent::_Ancestors($key, $fld, $inc);
                break;
            }

            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);
            $rfld = (!$flds ? $this->_rkf : (count($flds) == 1 ? $flds[0] : null));

            $f = $flds;
            $f[] = $this->_rkf;
            $f = join(',', $this->ProcessFlds($f));

            if ($rfld && $this->_mpm[$rfld]) {
                $mpm_f = $rfld;
                $mpm_p = $this->_mpm[$rfld];
            } else if ($this->_mpm[$this->_rkf]) {
                $mpm_f = $this->_rkf;
                $mpm_p = $this->_mpm[$this->_rkf];
            } else {
                reset($this->_mpm);
                list($mpm_f, $mpm_p) = each($this->_mpm);
            }

            // Get the specified record
            $rkv = $this->db->PrepareVal($key, $this->csf);
            $recs = $this->db->Fetch("SELECT $f,`{$mpm_p}`,`{$mpm_f}` FROM `{$this->_tab}` WHERE `{$this->_rkf}` = $rkv {$this->_tivSql}", 0, TNT_DB_FETCH_ASSOC, false);
            if (is_null($recs) || count($recs) != 1) {
                $this->_SetError('Can not select record: '.$key, $this->db->_GetError());
                break;
            }


            $path = $recs[0][$mpm_p];
            $base = $recs[0][$mpm_f];

            if ($path == '' || $path == '/') {
                // This is the root node so has no ancestors
                $ret = ($inc ? $recs : array());
                break;
            }

            // If a maintained path using the single requested record key field
            // then we can split it to get the array of ancestors.
            if ($rfld && $rfld == $mpm_f && $mpm_f == $this->_rkf) {
                $ret = array();
                foreach (explode('/', trim($path, '/')) as $p) {
                    $ret[] = array($mpm_f => $p);
                } 
                if ($inc) {
                    $ret[] = $recs;
                }
                break;
            }

            // If the maintained path uses the records primary key then
            // quickly fetch all by this key.
            if ($mpm_f == $this->_rkf) {
                // Quickest method as the record ids are materialized
                $ids = str_replace('/', ',', trim($path, '/'));

                // Get all the parent records
                $ret = $this->db->Fetch("SELECT $f FROM `{$this->_tab}` WHERE (`{$mpm_f}` IN ($ids)) {$this->_tivSql} ORDER BY LENGTH(`$mpm_p`)", 0, TNT_DB_FETCH_ASSOC, $this->csf);
            } else {
                // Otherwise generate a list of paths and select the records
                // based on those paths.
                $p = explode('/', trim($path, '/'));
                $ids = array();
                while ($p) {
                    $ids[] = "'/".join('/', $p)."/'";
                    array_pop($p);
                }
                $ids = join(',', $ids);
    
                // Get all the parent records
                $ret = $this->db->Fetch("SELECT $f FROM `{$this->_tab}` WHERE (CONCAT(`{$mpm_p}`, REPLACE(`{$mpm_f}`, '/', '_'), '/') IN ($ids)) {$this->_tivSql} ORDER BY LENGTH(`$mpm_p`)", 0, TNT_DB_FETCH_ASSOC, $this->csf);
            }

            if (is_null($ret)) {
                $this->_SetError('Can not select ancestors of record: '.$key, $this->db->_GetError());
                break;
            }

            if ($inc) {
                $ret[] = $recs[0];
            }

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
     * The '$key' argument should be the record key of a specific ancestor
     * record whose descendant should be returned. If you specify the virtual
     * root node key ($nkv) then you get the whole tree in a more optimised
     * fashion using a single database query.
     *
     * The '$fld' argument can be an array of field names or a comma separated
     * list of field names, in which case only the specified fields are fetched
     * for each record. The '*' field name can be used to mean all fields. The
     * '_depth' field name can be used to return the depth of each record from
     * the specified ancestor. The '_path' field name can be used to return
     * the absolute path of the record using the primary maintained path.
     * The records key field is always returned.
     *
     * The '$inc' argument can be used to cause the specified ancestor to be
     * included in the resultant records.
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
     * @return  array                   array of all sibling nodes
     * @see     Descendants()
     */
    function _Descendants($key, $fld = null, $inc = false, $ord = null, $pot = false) {
    
        $ret = null;

        // Start error section
        do {
            if (!$this->_mpm) {
                $ret = parent::Descendants($key, $fld, $inc, $ord, $pot);
                break;
            }

            reset($this->_mpm);
            list($mpm_f, $mpm_p) = each($this->_mpm);

            $flds = ($fld ? (is_array($fld) ? $fld : explode(',', $fld)) : null);

            $o = (is_null($ord) ? ($this->_cnf ? "r1.`{$this->_cnf}` ASC" : '') : join(',', $this->ProcessOrder($ord, 'r1')));
            if ($o) {
                $o = "ORDER BY $o";
            }

            $show_path = ($flds && ($flds[0] == '*' || in_array('_path', $flds)));

            $f = $flds;
            $f[] = $this->_rkf;
            if (!$show_path || !$o) {
                $f[] = '_path';
            }
            $f = join(',', $this->ProcessFlds($f, 'r1'));

            if ($o) {
                $f .= ", r1.`{$this->_pkf}`";
            }

            $rkv = $this->db->PrepareVal($key, $this->csf);

            $sel_key = ($key == $this->_nkv ? $this->_pkf : $this->_rkf);

            $inc_sql = ($inc ? "(r1.`{$sel_key}` = $rkv) OR " : '');

            $tivSql = '';
            if ($this->_tif) {
                $tivSql = preg_replace('/^AND /', 'AND r1.', $this->_tivSql);
                $tivSql .= preg_replace('/^AND /', ' AND r2.', $this->_tivSql);
            }

            $sql = "SELECT $f FROM `{$this->_tab}` r1, `{$this->_tab}` r2 WHERE ({$inc_sql}(r1.`{$mpm_p}` LIKE CONCAT(r2.`{$mpm_p}`, REPLACE(r2.`{$mpm_f}`, '/', '_'), '/%'))) AND r2.`{$sel_key}` = $rkv {$tivSql} ".($o ? $o : 'ORDER BY _path');
            // Get the specified records descendant (optionally include itself)
            $recs = $this->db->Fetch($sql, 0, TNT_DB_FETCH_ASSOC, false);
            if (is_null($recs)) {
                $this->_SetError('Can select descendant of record: '.$key, $this->db->_GetError());
                break;
            }

            // Remove the path field used to order the query unless we asked
            // for it
            if (!$o && !$show_path) {
                $max_i = count($recs);
                for ($i = 0; $i < $max_i; $i++) {
                    unset($recs[$i]['_path']);
                }
            }

            if ($o && $pot) {
                // When we use a sort order the recs don't come out in
                // hierachical order - so we have to process them into
                // the correct order.
                $tree = array();
                $top_key = $key;
                if ($inc) {
                    $max_i = count($recs);
                    for ($i = 0; $i < $max_i; $i++) {
                        if ($recs[$i][$this->_rkf] == $key) {
                            $top_key = $recs[$i][$this->_pkf];
                        }
                    }
                }
                if (!$this->_PreOrderTreeSort($top_key, $recs, count($recs), 0, $tree)) {
                    break;
                }

                $recs = $tree;

                // Remove the parent key field unless we asked for it.
                $show_pkf = ($flds && ($flds[0] == '*' || in_array($this->_pkf, $flds)));
                if (!$show_pkf) {
                    $max_i = count($recs);
                    for ($i = 0; $i < $max_i; $i++) {
                        unset($recs[$i][$this->_pkf]);
                    }
                }

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
     * the specified ancestor. The records key field is always returned.
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
    
        // Where there is a parent key we can't do this any faster using
        // maintained paths.
        return parent::_Siblings($key, $fld, $inc, $ord);
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
     * If $path is passed as a string value then it is assumed to be a
     * string of key values separated by a '/' character.
     *
     * This method differs from the base {@link TNTDbNode::PathToKey()} method
     * in that it will select the record using any materialized path available
     * for the selected field. This means the key can be found in a single
     * database query.
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

            if (!isset($this->_mpm[$fld])) {
                $ret = parent::PathToKey($path, $fld);
                break;
            }

            if (is_string($path)) {
                $path = explode('/', trim($path, '/'));
            }

            // Use the path to select the record - single database query

            $mpm_p = $this->_mpm[$fld];
            $mpm_f = $fld;

            $cb = create_function('$p', 'return str_replace(\'/\', \'_\', $p);');

            $p = $this->db->QuoteString('/'.join('/', array_map($cb, $path)).'/');

            $key = $this->db->FetchValue("SELECT `{$this->_rkf}` FROM `{$this->_tab}` WHERE (CONCAT(`{$mpm_p}`, REPLACE(`{$mpm_f}`, '/', '_'), '/') = $p) {$this->_tivSql}", $this->csf);
            if (is_null($key)) {
                $this->_SetError('Can not select a record with path: '.$p, $this->db->_GetError());
                break;
            }

            $ret = $key;

        } while (false);
        /* End error section */

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
            if ($this->_ndf || !$this->_mpm) {
                $ret = parent::Depth($key);
                break;
            }

            if (isset($this->__depth[$key])) {
                $ret = $this->__depth[$key];
                break;
            }

            reset($this->_mpm);
            list($mpm_f, $mpm_p) = each($this->_mpm);

            $f = "";

            $rkv = $this->db->PrepareVal($key, $this->csf);

            $recs = $this->db->Fetch("SELECT (LENGTH(`{$mpm_p}`) - LENGTH(REPLACE(`{$mpm_p}`, '/', ''))) - 1 AS _depth FROM `{$this->_tab}` WHERE `{$this->_rkf}` = $rkv {$this->_tivSql}", 0, TNT_DB_FETCH_ASSOC, false);
            if (is_null($recs)) {
                $this->_SetError('Can select depth of record: '.$key, $this->db->_GetError());
                break;
            }
            if (count($recs) != 1) {
                $this->_SetError('No such record: '.$key);
                break;
            }

            $ret = $recs[0]['_depth'];

            $this->__depth[$key] = $ret;

        } while (false);
        // End error section

        return $ret;
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
     * The order value field is option and if not specified it will not be
     * used and no child ordering will be supported.
     *
     * A materialized path map can be specified as an array of mappings in the
     * format ('<elementFld>' => '<cacheFld>'), where <elementFld> is the field
     * in the record that makes up each level in the path and <cacheFld> is the
     * field in the record in which the complete path is cached (materialized).
     *
     * The <elementFld> must be unique with in its parent, that is
     * (<parentKeyFld>,<elementFld) must be a unqiue key, however it need
     * not be defined as a key in the database schema.
     *
     * @access  public
     * @param   string     $table       pseudo database table name
     * @param   string     $rkf         record key field name in table
     * @param   string     $pkf         parent foriegn key field name in table
     * @param   array      $mpm         maintain path map
     * @param   array      $options     construction options (optional)
     * @return  void
     */
    function TNTDbMpNode($table, $rkf = null, $pkf = null, $mpm = null, $options = null) {

        TNTDbNode::TNTDbNode($table, $rkf, $pkf, $options);

        $this->_mpm = null;

        foreach ($mpm as $e => $m) {
            if ($e == $m) {
                TNT::Abort("TNTDbRec::TNTDbRec() - path element ($e) can not be same as path cache ($m)");

            } else if (!isset($this->_schema[$e])) {
                TNT::Abort("TNTDbRec::TNTDbRec() - path element ($e) is not in table schema");
            } else if (!isset($this->_schema[$m])) {
                TNT::Abort("TNTDbRec::TNTDbRec() - path cache ($m) is not in table schema");
            } else if (in_array($e, $mpm)) {
                TNT::Abort("TNTDbRec::TNTDbRec() - path element ($e) can not also be a path cache");
            } else {
                $this->_mpm[$e] = $m;
            }
        }
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTDbMpNode() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
