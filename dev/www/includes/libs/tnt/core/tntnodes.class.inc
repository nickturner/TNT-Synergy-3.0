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
 * Nodes Class
 * ~~~~~~~~~~~
 *
 * This is a small class that manages a collection of nodes.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObjects');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to manage a collection of nodes.
 *
 * This class is used to manage a collection of nodes, it derives from
 * TNTObjects and contains an array of managed TNTNode derived objects.
 *
 * The collection can be readonly in which case the Add and Remove calls
 * will fail.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @extends TNTObjects
 * @iface   TNTICollection
 */
class TNTNodes extends TNTObjects {
    /**
     * Owning node
     * @var     TNTNode                 reference to owner node
     * @access  private
     */
    var $_owner = null;

    /**
     * Called after a node is added to the collection
     *
     * Notifies owning node that a child has been added to it
     *
     * @access  protected
     * @param   TNTNode     $node       reference to the node added
     * @param   integer     $index      index at which the node was added
     * @return  void
     * @see     Add(), AddAt(), AddAll(), AddAllAt()
     */
    function _Added(&$node, $index) {
        if ($this->_owner) {
            $this->_owner->_AddedChild($node, $index);
        }
    }

    /**
     * Called after a node is removed from the collection
     *
     * Notifies owning node that a child has been removed from it
     *
     * @access  protected
     * @param   TNTNode     $node       reference to the node removed
     * @param   integer     $index      index from which the node was removed
     * @return  void
     * @see     Remove(), RemoveAt(), RemoveAll(), RemoveAllAt()
     */
    function _Removed(&$node, $index) {
        if ($this->_owner) {
            $this->_owner->_RemovedChild($node, $index);
        }
    }

    /**
     * Tests to see if the given object is one of the required classes.
     *
     * Same as base class only ensures that the object is definately a
     * a 'TNTNode' object.
     *
     * @access  protected
     * @abstract
     * @param   TNTObject&  $object     reference to the object
     * @return  boolean                 true if object is of required class
     */
    function IsReqClass(&$object) {
        if (!is_a($object, 'TNTNode')) {
            return false;
        } else if ($this->reqClasses && !is_kindof($object, $this->reqClasses)) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * Construct the collection node.
     *
     * This will construct the base TNTObjects class and set the class
     * to only accept children derived from the class 'TNTNode'.
     *
     * @access  public
     */
    function TNTNodes() {

        TNTObject::TNTObject();

        $this->reqClasses = array('TNTNode');

    }

    /**
     * Destruct the collection node.
     *
     * This will destruct the collection node.
     *
     * @access  private
     */
    function _TNTNodes() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
