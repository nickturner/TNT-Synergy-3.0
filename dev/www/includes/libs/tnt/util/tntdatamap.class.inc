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
 * Data Map Class 
 * ~~~~~~~~~~~~~~
 *
 * This is a small class that can manipulate arrays of data according to
 * a set of instructions.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObject');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to map and unmap arrays of data.
 *
 * The data is given as an array of records indexed by some primary key. Each
 * record is an associative array of data.
 *
 * The primary key can be any scalar value and need not be sequential.
 *
 * The mapping changes column names and can create heirachical data structures.
 *
 * The mapping definition is a simple URL encoded query string of the format
 * 'x=y&x=y..' etc. where 'x' is the old column name and 'y' is the new name
 * for the column. If 'y' is blank the the column is deleted.
 *
 * Certain 'x' values can specify transformations, these use a name prefixed
 * with a '_' character as follows;
 *
 *   + '_index' - index the data using the value of column 'y' as the index.
 *
 *   + '_rindex' - make the value of column 'y' equal to the index.
 *
 *   + '_heirachy' - specifies that the data should be reduced to a heirachy.
 *     'y' is the 'child,parent' key column names (separated by a comma). Each
 *     record is examined in turn and its child records are moved to an array
 *     stored in a column called '_children'. The returned array of data is
 *     therefore reduced to an array of root records (that have no parent).
 *
 *   + '_promote' - if 'y' is an array then all its values are merged into the
 *     record.
 *
 *   + '_duplicate' - the named column should be duplicated. 'y' is the original
 *     and duplicate column names in the format 'original,duplicate'. The
 *     duplicate column is made as a reference to the original column.
 *
 * If numeric values are give for 'x' or 'y' then they are assumed to be numeric
 * array indices and not string indices.
 *
 * The mappings are done in the following order;
 *
 *   1) Indexing specified by '_index'.
 *   2) Indexing specified by '_rindex'.
 *   3) Promoting specified by '_promote'.
 *   3) Duplication specified by '_duplicate'.
 *   4) Parenting specified by the '_id' and '_pid' combination.
 *   5) Column renaming (removing) specified by 'x=y' or 'x=' (in map order).
 *
 * Renaming and removing is performed last so that the columns referenced by
 * the transformations '_index' and '_heirachy' are the original names and can
 * be removed if required after the transformations.
 *
 * @package TNT.Util
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTDataMap extends TNTObject {

    /**
     * The data map
     * @var     string                  data map
     * @access  public
     */
    var $dataMap = false;

    /**
     * Rename columns
     *
     * Apply column renaming (removing) to the given data.
     *
     * Works recursively if a column called '_children' is found and is
     * not removed by the column mapping. The mapping can rename the
     * '_children' column.
     *
     * @access  public
     * @param   array[]&    $data       array of records to rename columns
     * @param   array&      $map        the old to new name mappings
     * @return  void
     */
    function _MapCols(&$data, &$map) {
        $_children = '_children';
        $keys = array_keys($data);
        foreach ($keys as $k) {
            foreach ($map as $oc => $nc) {
                if (isset($data[$k][$oc])) {
                    if ($nc != '') {
                        if (isset($data[$k][$nc])) {
                            settype($data[$k][$nc], 'array');
                            $data[$k][$nc][] = $data[$k][$oc];
                        } else {
                            $data[$k][$nc] = $data[$k][$oc];
                        }
                    }
                    unset($data[$k][$oc]);
                }
                if ($oc == $_children) {
                    $_children = $nc;
                }
            }

            if ($_children != '' && isset($data[$k][$_children])) {
                $this->_MapCols($data[$k][$_children], $map);
            }
        }
    }

    /**
     * Gather the children for each of the given records.
     *
     * Children are deleted from the pool as they are gathered up.
     *
     * @access  public
     * @param   array[]     $data       the records to gather children of
     * @param   string      $idcol      name of records id column
     * @param   string      $pidcol     name of records parentid column
     * @param   array[]     $pool       the records to find the children in
     * @return  void
     */
    function _GatherChildren(&$data, $idcol, $pidcol, &$pool) {
        $data_keys = array_keys($data);
        $pool_keys = array_keys($pool);

        // For each record
        foreach ($data_keys as $k) {
            $id = $data[$k][$idcol];
            $children = null;
            // Find all its children (ie. those whos pid == id)
            foreach ($pool_keys as $p => $k1) {
                if ($pool[$k1][$pidcol] == $id) {
                    $children[$k1] =& $pool[$k1];
                    unset($pool[$k1]);
                    unset($pool_keys[$p]);
                }
            }

            $data[$k]['_children'] = $children;

            if ($children) {
                // Collect the children of these children
                $this->_GatherChildren($data[$k]['_children'], $idcol, $pidcol, $pool);
                $pool_keys = array_keys($pool);
            }
        }
    }

    /**
     * Map the data.
     *
     * Each column is renamed as required and then any transformation are
     * applied.
     *
     * @access  public
     * @param   array[]                 the array of data to map
     * @return  array[]                 the array of mapped data
     */
    function Map($data) {

        $ret = null;

        $map = null;
        $colmap = null;
        $idxcol = null;
        $ridxcol = null;
        $idcol = null;
        $pidcol = null;
        $expcols = null;
        $dupcols = null;

        // Start error section
        do {
            $map = urlquery_decode($this->dataMap);
            if (!$map) {
                $this->_SetError('Failed to parse the data map specification: '.$this->dataMap);
                $ret = null;
                break; // break on error
            }
            if (count($map) == 0 ||
                !is_array($data) || count($data) == 0 ||
                !is_array($data[0])) {
                // no data to map - return unchanged
                $ret = $data;
                break; // break on completion
            }

            // Do some initial validation
            if (isset($map['_index'])) {
                $idxcol = $map['_index'];
                if (!isset($data[0][$idxcol])) {
                    $this->_SetError("Index column '$idxcol' does not exist");
                    $ret = null;
                    break; // break on error
                }
            }

            if (isset($map['_rindex'])) {
                $ridxcol = $map['_rindex'];
            }

            if (isset($map['_promote'])) {
                $expcols = explode(',', $map['_promote']);
                foreach ($expcols as $c) {
                    if (!isset($data[0][$c])) {
                        $this->_SetError("Index column '$c' does not exist");
                        $expcols = false;
                        break; // break on error
                    }
                }
                if (!$expcols) {
                    $ret = null;
                    break;
                }
            }

            if (isset($map['_duplicate'])) {
                $dupcols = explode(',', $map['_duplicate']);
                $max_dupcols = count($dupcols);
                if ($max_dupcols % 2 != 0) {
                    $this->_SetError('Duplicate requires pairs of column names: '.$map['_duplicate']);
                    $dupcols = false;
                    break; // break on error
                }
                for ($i = 0; $i < $max_dupcols; $i += 2) {
                    if (!isset($data[0][$dupcols[$i]])) {
                        $this->_SetError("Index column '{$dupcols[$i]}' does not exist");
                        $dupcols = false;
                        break; // break on error
                    }
                }
                if (!$dupcols) {
                    break; // break on error
                }
            }

            if (isset($map['_heirachy'])) {
                $c = explode(',', $map['_heirachy'], 2);
                if (count($c) == 1) {
                    $idcol = $idxcol;
                    $pidcol = $c[0];
                } else {
                    $idcol = $c[0];
                    $pidcol = $c[1];
                }
                if (!isset($data[0][$idcol])) {
                    $this->_SetError("Id column '$idcol' does not exist");
                    $ret = null;
                    break; // break on error
                }
                if (!isset($data[0][$pidcol])) {
                    $this->_SetError("ParentId column '$pidcol' does not exist");
                    $ret = null;
                    break; // break on error
                }
            }

            $colmap = null;
            foreach ($map as $oc => $nc) {
                if ($oc{0} != '_') {
                    if (is_numeric($oc)) {
                        $oc = (int)$oc;
                    }
                    if ($nc != '' && is_numeric($nc)) {
                        $nc = (int)$nc;
                    }
                    $colmap[$oc] = $nc;
                }
            }

            // Index the data via a key
            if (!is_null($idxcol)) {
                $new = null;
                foreach (array_keys($data) as $k) {
                    $new[$data[$k][$idxcol]] =& $data[$k];
                }
                $data = $new;
            }

            // Get an array of keys to the records - this is used to iterate
            // over the records without having to call methods like foreach
            // or each which make copies of the record or records. Thus this
            // saves memory.
            $keys = array_keys($data);
            $max_keys = count($keys);

            // Copy index to the data column
            if (!is_null($ridxcol)) {
                for ($k = 0; $k < $max_keys; $k++) {
                    $data[$keys[$k]][$ridxcol] = $k;
                }
            }

            // Promote the given columns
            if (!is_null($expcols)) {
                for ($k = 0; $k < $max_keys; $k++) {
                    $max_i = count($expcols);
                    for ($i = 0; $i < $max_i; $i++) {
                        if (is_array($data[$key[$k]][$expcols[$i]])) {
                            $d = $data[$key[$k]][$expcols[$i]];
                            unset($data[$key[$k]][$expcols[$i]]);
                            $data[$keys[$k]] = array_merge($data[$keys[$k]], $d);
                        }
                    }
                }
            }

            // Duplicate specified columns
            if (!is_null($dupcols)) {
                for ($k = 0; $k < $max_keys; $k++) {
                    $max_i = count($expcols);
                    for ($i = 0; $i < $max_i; $i += 2) {
                        $data[$keys[$k]][$expcols[$i + 1]] = $data[$key[$k]][$expcols[$i]];
                    }
                }
            }

            // Convert to heirachical tree
            if (!is_null($pidcol)) {
                $new = null;
                // First find all root records - ones with no parent
                for ($k1 = 0; $k1 < $max_keys; $k1++) {
                    $d1 =& $data[$keys[$k1]];
                    $pid = $d1[$pidcol];
                    $has_parent = false;
                    for ($k2 = 0; $k2 < $max_keys; $k2++) {
                        $d2 =& $data[$keys[$k2]];
                        if ($d2[$idcol] == $pid) {
                            $has_parent = true;
                            break;
                        }
                        unset($d2);
                    }
                    if (!$has_parent) {
                        $new[] =& $data[$k1];
                    }
                    unset($d1);
                }

                if ($new) {
                    // Foreach root node gather its children
                    $this->_GatherChildren($new, $idcol, $pidcol, $data);
                }

                $data = $new;
            }

            // Rename (remove) the columns
            if ($colmap) {
                $this->_MapCols($data, $colmap);
            }

            $ret =& $data;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * UnMap the data
     *
     * Reverses anything a call to {@link Map()} performed.
     *
     * @access  public
     * @param   array[]                 the array of data to unmap
     * @return  array[]                 the array of unmapped data
     */
    function UnMap(&$data) {
        return null;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @param   string      $dataMap    the data mapping specification
     */
    function TNTDataMap($dataMap) {

        TNTObject::TNTObject();

        $this->dataMap = $dataMap;

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTDataMap() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
