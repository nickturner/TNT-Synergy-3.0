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
 * Node Class 
 * ~~~~~~~~~~
 *
 * This is a small class that all nodes are derived from.
 *
 * @package TNT.Core
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
 * Class to represent a node in a heirachy of objects.
 *
 * All objects in the heirachy should be derived from this object. The class
 * gives methods to search the heirachy etc.
 *
 * @package TNT.Core
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTNode extends TNTObject {

    /**
     * Parent node
     * @access  public
     * @var     TNTNode                 reference to parent node
     */
    var $parent = null;

    /**
     * Child nodes
     * @access  public
     * @var     TNTNodes                collection of child nodes
     */
    var $children = null;

    /**
     * Array of member names to inherit from parent node
     * Automatic references to these members are created as members of this node
     * ($root is automatically inherited)
     * @access  public
     * @var     string[]                array of member names
     */
    var $inheritMembers = null;

    /**
     * All child nodes must be derived from one of the class names given in
     * this list. All specified classes must be derived from {@link TNTNode}.
     * @var     string[]                array of class name(s) (optional)
     * @access  public
     */
    var $reqChildClasses = array('TNTNode');

    /**
     * Root node
     * @access  private
     * @var     TNTNode                 reference to root node
     */
    var $__root = null;

    /**
     * Serialize the object
     *
     * This function returns the object as a character string.
     *
     * Overridden from base class to prevent the serialisation of the
     * parent member variable so that only the subtree starting at this
     * node is actually serialized and not the whole tree that this node
     * belongs too.
     *
     * This method is hookable.
     *
     * @access  public
     * @return  string                  string of serialized object data
     * @see     UnSerialize()
     */
    function Serialize() {
        $tmp =& $this->parent;

        // Serialized objects are wrapped in an array to overcome having to
        // pass in a reference to the object to unserialize() when force pass
        // by reference is disabled.
        $ret = serialize(array(&$this));

        $this->parent =& $tmp;

        return $ret;
    }

    /**
     * Called when node is added or removed from a collection
     *
     * Notifies node that it is now a child of the parent node
     *
     * The node can be configured to automatically inherit variables
     * from the parent node, when adopted these variables are inherited.
     *
     * @access  protected
     * @param   TNTNode     $parent     reference to the parent node
     * @return  void
     * @see     _Orphaned(), _AddedChild(), _RemovedChild()
     */
    function _Adopted(&$parent) {
        $this->parent =& $parent;

        if ($this->inheritMembers) {
            foreach ($this->inheritMembers as $member) {
                $this->$member =& $this->parent->$member;
            }
        }

        // Clear cached values
        $this->__root = null;
    }

    /**
     * Called when node is removed from a collection
     *
     * Notifies node that it is now an orphan (has no parent).
     *
     * The node can be configured to automatically inherit variables
     * from the parent node, when orphaned these variables are lost.
     *
     * @access  protected
     * @return  void
     * @see     _Adopted(), _AddedChild(), _RemovedChild()
     */
    function _Orphaned(&$parent) {
        $this->parent = null;

        if ($this->inheritMembers) {
            foreach ($this->inheritMembers as $member) {
                $this->$member = null;
            }
        }

        // Clear cached values
        $this->__root = null;
    }

    /**
     * Called after a node is added to the collection
     *
     * Notifies owning node that a child has been added to it
     *
     * @access  protected
     * @param   TNTNode     $child      reference to the node added
     * @param   integer     $index      index at which the node was added
     * @return  void
     * @see     _Adopted(), _Orphaned(), _RemovedChild()
     */
    function _AddedChild(&$child, $index) {
        $child->_Adopted($this);
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
     * @see     _Adopted(), _Orphaned(), _AddedChild()
     */
    function _RemovedChild(&$child, $index) {
        $child->_Orphaned(null);
    }

    /**
     * Creates a new TNTNodes object to hold the child nodes.
     *
     * Override this method in a derived node if you have defined a collection
     * object that is derived from the TNTNodes class. You can then instantiate
     * that derived class in the override of this method.
     *
     * This default method creates a TNTNodes() object to store the children
     * and the called {@link ValidChildClasses()} to configure it to store
     * only a given set of object types.
     * 
     * @access  private
     * @return  TNTNodes                object to contain the nodes children
     */
    function &CreateChildCollection() {
        TNTLoader::RequireClass('TNTNodes');
        $ret =& new TNTNodes();
        $ret->reqClasses = $this->reqChildClasses;
        $ret->autoDestroy = true;
        return $ret;
    }

    /**
     * Check the specified property.
     *
     * The function returns true if the given property equals the given
     * value.
     *
     * The property is a member variable of the node. The following special
     * pseudo property names are recognised that don't represent actual node
     * member variables.
     *   + '!class' - represents the node class name (lowercase)
     *   + '!parentclass' - represents the nodes parent class name (lowercase)
     *
     * '$strict' cause strict value and type matching. When checking class
     * names '$strict' can be used to restrict the class match to exact
     * classes and not derived classes.
     *
     * @access  public
     * @param   mixed       $value      value the property should have
     * @param   string      $property   name of property to check
     * @param   boolean     $strict     value must type match
     * @return  true                    true if property equals given value
     * @see     FindAncestors(), FindDescendants(), FindSiblings()
     */
    function PropertyIsEqual($value, $property = 'id', $strict = false) {
        switch ($property) {
            case '!class':
                $ret = (($strict && $this->IsClassOf($value)) ||
                        (!$strict && $this->IsSubClassOf($value)));
                break;

            case '!parentclass':
                $ret = (get_parent_class($this) == strtolower($value));
                break;
                
            default:
                if (!isset($this->$property)) {
                    $ret = false;
                    break;
                }
                $ret = (($strict && ($this->$property === $value)) ||
                        (!$strict && ($this->$property == $value)));
                break;
        }

        return $ret;
    }

    /**
     * Get root node
     *
     * Finds the root node (that is an node with no parent)
     *
     * The return value from this function is cached, so although the
     * first call will calculate the root, subsequent calls will return
     * the calculated value. The cache is cleared if the node is orphaned.
     *
     * @access  public
     * @return  TNTNode                 reference to root node
     */
    function &Root() {
        if (!$this->__root) {
            if (!$this->parent) {
                $this->__root =& $this;
            } else {
                $this->__root = &$this->parent;
                while ($this->__root->parent) {
                    $this->__root =& $this->__root->parent;
                }
            }
        }
        return $this->__root;
    }

    /**
     * Walk the node hierachy calling the given call back on each child
     * of the node.
     *
     * The callback will be called with two arguments, the first will be a
     * reference to the current node in the hierachy and the second will be
     * any user supplied $data value. If the callback returns false then the
     * walk will stop amd the method will return the number of nodes walked
     * as a negative value. That is if 5 nodes were walked when the callback
     * return false then the method will return -5 as opposed to +5.
     *
     * The nodes are walked using a left to right direction.
     *
     * @access  public
     * @param   callback    $callback   callback to call on each node
     * @param   mixed       $data       data to pass as second arg to callback
     * @return  integer                 number of nodes walked
     * @see     WalkAncestors(), WalkDescendants(), WalkSiblings()
     */
    function WalkChildren($callback, $data) {
        $ret = 0;
        if (($max_c = $this->children->Count())) {
            $children =& $this->children->Items();
            $func_args = array(null, $data);
            for ($c = 0; $c < $max_c; ++$c) {
                ++$ret;
                $func_args[0] =& $children[$c];
                if (!call_user_func_array($callback, $func_args)) {
                    $ret = -$ret;
                    break;
                }
            }
        }

        return $ret;
    }

    /**
     * Call the given method on all child nodes.
     *
     * The nodes are walked using a left to right traversal of the hierachy.
     *
     * @access  public
     * @param   string      $method     name of method to call
     * @param   array       $args       array of arguments to pass to method
     * @return  integer                 number of nodes called
     * @see     CallAncestors(), CallDescendants(), CallSiblings()
     */
    function &CallChildren($method, $args) {
        $cb = create_function('&$node, &$data',
            'call_user_func(array(&$node, $data[0], $data[1])); return true;');
        $data = array($method, $args);
        $ret = $this->WalkChildren($cb, $data);
        return $ret;
    }

    /**
     * Searches children for nodes with the matching property.
     *
     * The method will return all children where a call to
     * {@link TNTSmartNode::PropertyIsEqual()} with the specified arguments
     * returns true.
     *
     * This function returns null if the node has no children or no
     * children that match the requirements.
     *
     * The returned collection contains the children in the order found when
     * traversing left to right on the node.
     *
     * @access  public
     * @param   mixed       $value      value the property should have
     * @param   string      $property   name of property to check
     * @param   boolean     $strict     value must type match
     * @return  TNTNode[]               array of references to ancestors
     * @see     FindAncestors(), FindDescendants(), FindSiblings()
     */
    function &FindChildren($value, $property = 'id', $strict = false) {
        $ret = null;
        $cb = create_function('&$node, &$data',
            'if ($node->PropertyIsEqual($data[0], $data[1], $data[2])) { $data[3][] =& $node; }; return true;');
        $data = array($value, $property, $strict, &$ret);
        $this->WalkChildren($cb, $data);
        return $ret;
    }

    /**
     * Returns an array of all children
     *
     * The nodes are returned using a left to right traversal of the hierachy.
     *
     * Modifying this array will not modify the child nodes, but modifying the
     * children referenced in this array will modify the actual child nodes.
     *
     * @access  public
     * @return  TNTNode[]               array of references to siblings
     * @see     Ancestors(), Descendants(), Siblings()
     */
    function Children() {
        $ret = ($this->children->Count() ?  $this->children->Items() : null);
        return $ret;
    }

    /**
     * Walk the node hierachy calling the given call back on each ancestor
     * of the node.
     *
     * The callback will be called with two arguments, the first will be a
     * reference to the current node in the hierachy and the second will be
     * any user supplied $data value. If the callback returns false then the
     * walk will stop amd the method will return the number of nodes walked
     * as a negative value. That is if 5 nodes were walked when the callback
     * return false then the method will return -5 as opposed to +5.
     *
     * The nodes are walked using a leaf to root direction up the hierachy.
     *
     * @access  public
     * @param   callback    $callback   callback to call on each node
     * @param   mixed       $data       data to pass as second arg to callback
     * @return  integer                 number of nodes walked
     * @see     WalkChildren(), WalkDescendants(), WalkSiblings()
     */
    function WalkAncestors($callback, $data) {
        $ret = 0;
        $func_args = array(null, $data);
        for ($p = &$this->parent; $p; $p = &$p->parent) {
            ++$ret;
            $func_args[0] =& $p;
            if (!call_user_func_array($callback, $func_args)) {
                $ret = -$ret;
                break;
            }
        }
        return $ret;
    }

    /**
     * Call the given method on all ancestor nodes.
     *
     * The nodes are called using a leaf to root direction up the hierachy.
     *
     * @access  public
     * @param   string      $method     name of method to call
     * @param   array       $args       array of arguments to pass to method
     * @return  integer                 number of nodes called
     * @see     CallChildren(), CallDescendants(), CallSiblings()
     */
    function &CallAncestors($method, $args) {
        $ret = 0;
        $cb = create_function('&$node, &$data',
            'call_user_func(array(&$node, $data[0], $data[1])); return true;');
        $data = array($method, $args);
        $ret = $this->WalkAncestors($cb, $data);
        return $ret;
    }

    /**
     * Searches ancestory for nodes with the matching property.
     *
     * The method will return all ancestors where a call to
     * {@link TNTSmartNode::PropertyIsEqual()} with the specified arguments
     * returns true.
     *
     * This function returns null if the node has no ancestors or no
     * ancestors that match the requirements.
     *
     * The returned collection contains the ancestors in the order found when
     * traversing up the tree.
     *
     * @access  public
     * @param   mixed       $value      value the property should have
     * @param   string      $property   name of property to check
     * @param   boolean     $strict     value must type match
     * @return  TNTNode[]               array of references to ancestors
     * @see     FindChildren(), FindDescendants(), FindSiblings()
     */
    function &FindAncestors($value, $property = 'id', $strict = false) {
        $ret = null;
        $cb = create_function('&$node, &$data',
            'if ($node->PropertyIsEqual($data[0], $data[1], $data[2])) { $data[3][] =& $node; }; return true;');
        $data = array($value, $property, $strict, &$ret);
        $this->WalkAncestors($cb, $data);

        return $ret;
    }

    /**
     * Returns an array of all ancestors
     *
     * The returned collection contains the ancestors in the order found when
     * traversing up the tree.
     *
     * @access  public
     * @return  TNTNode[]               array of references to ancestors
     * @see     Children(), Descendants(), Siblings()
     */
    function &Ancestors() {
        $ret = null;

        $cb = create_function('&$node, &$data',
            '$data[0][] =& $node; return true;');
        $data = array(&$ret);

        $cnt = $this->WalkAncestors($cb, $data);

        return $ret;
    }

    /**
     * Walk the node hierachy calling the given call back on each descendant
     * of the node.
     *
     * The callback will be called with two arguments, the first will be a
     * reference to the current node in the hierachy and the second will be
     * any user supplied $data value. If the callback returns false then the
     * walk will stop amd the method will return the number of nodes walked
     * as a negative value. That is if 5 nodes were walked when the callback
     * return false then the method will return -5 as opposed to +5.
     *
     * The nodes are walked using a left to right traversal down the hierachy.
     *
     * @access  public
     * @param   callback    $callback   callback to call on each node
     * @param   mixed       $data       data to pass as second arg to callback
     * @return  integer                 number of nodes walked
     * @see     WalkChildren(), WalkAncestors(), WalkSiblings()
     */
    function WalkDescendants($callback, $data) {
        $ret = 0;
        if (($max_c = $this->children->Count())) {
            $children =& $this->children->Items();
            $func_args = array(null, $data);
            for ($c = 0; $c < $max_c; ++$c) {
                ++$ret;
                $func_args[0] =& $children[$c];
                if (!call_user_func_array($callback, $func_args)) {
                    $ret = -$ret;
                    break;
                }
                if ($children[$c]->children->Count()) {
                    $cnt = $children[$c]->WalkDescendants($callback, $data);
                    $ret += abs($cnt);
                    if ($cnt < 0) {
                        $ret = -$ret;
                        break;
                    }
                }
            }
        }

        return $ret;
    }

    /**
     * Searches descendants for nodes with the given property.
     *
     * The method will return all descendants where a call to
     * {@link TNTSmartNode::PropertyIsEqual()} with the specified arguments
     * returns true.
     *
     * This function returns null if the node has no descendants or no
     * descendants that match the requirements.
     *
     * The returned collection contains the descendants in the order found when
     * doing a left to right traversal down the tree.
     *
     * @access  public
     * @param   mixed       $value      value the property should have
     * @param   string      $property   name of property to check
     * @param   boolean     $strict     value must type match
     * @return  TNTNode[]               array of references to descendants
     * @see     FindChildren(), FindAncestors(), FindSiblings()
     */
    function &FindDescendants($value, $property = 'id', $strict = false) {
        $ret = null;
        $cb = create_function('&$node, &$data',
            'if ($node->PropertyIsEqual($data[0], $data[1], $data[2])) { $data[3][] =& $node; }; return true;');
        $data = array($value, $property, $strict, &$ret);
        $this->WalkDescendants($cb, $data);

        return $ret;
    }

    /**
     * Call the given method on all descendant nodes.
     *
     * The method will be called on all the descendants in the order found
     * when doing a left to right traversal down the tree.
     *
     * @access  public
     * @param   string      $method     name of method to call
     * @param   array       $args       array of arguments to pass to method
     * @return  integer                 number of nodes called
     * @see     CallChildren(), CallAncestors(), CallSiblings()
     */
    function &CallDescendants($method, $args) {
        $ret = null;
        $cb = create_function('&$node, &$data',
            'call_user_func(array(&$node, $data[0], $data[1])); return true;');
        $data = array($method, $args);
        $ret = $this->WalkDescendants($cb, $data);

        return $ret;
    }

    /**
     * Returns an array of all descendants
     *
     * The returned collection contains the descendants in the order found when
     * doing a left to right traversal down the tree.
     *
     * @access  public
     * @return  TNTNode[]               array of references to descendants
     * @see     Children(), Ancestors(), Siblings()
     */
    function &Descendants() {
        $ret = null;
        $cb = create_function('&$node, &$data',
            '$data[0][] =& $node; return true;');
        $data = array(&$ret);

        $this->WalkDescendants($cb, $data);

        return $ret;
    }

    /**
     * Walk the node hierachy calling the given call back on each sibling
     * of the node.
     *
     * The callback will be called with two arguments, the first will be a
     * reference to the current node in the hierachy and the second will be
     * any user supplied $data value. If the callback returns false then the
     * walk will stop amd the method will return the number of nodes walked
     * as a negative value. That is if 5 nodes were walked when the callback
     * return false then the method will return -5 as opposed to +5.
     *
     * The nodes are walked using a left to right traversal of the hierachy.
     *
     * @access  public
     * @param   callback    $callback   callback to call on each node
     * @param   mixed       $data       data to pass as second arg to callback
     * @return  integer                 number of nodes walked (or -1)
     * @see     WalkChildren(), WalkAncestors(), WalkDescendants()
     */
    function WalkSiblings($callback, $data) {
        $ret = 0;
        if ($this->parent) {
            $children =& $this->parent->Children();
            $max_c = count($children);
            $func_args = array(null, $data);
            for ($c = 0; $c < $max_c; ++$c) {
                if ($children[$c] !== $this) {
                    ++$ret;
                    $func_args[0] =& $children[$c];
                    if (!call_user_func_array($callback, $func_args)) {
                        $ret = -$ret;
                        break;
                    }
                }
            }
        }

        return $ret;
    }

    /**
     * Searches siblings for nodes with the given property.
     *
     * The method will return all siblings where a call to
     * {@link TNTSmartNode::PropertyIsEqual()} with the specified arguments
     * returns true.
     *
     * This function returns null if the node has no siblings or no
     * siblings that match the requirements.
     *
     * The returned collection contains the siblings in the order found when
     * doing a left to right traversal across the parent node.
     *
     * @access  public
     * @param   mixed       $value      value the property should have
     * @param   string      $property   name of property to check
     * @param   boolean     $strict     value must type match
     * @return  TNTNode[]               array of references to siblings
     * @see     FindChildren(), FindAncestors(), FindDescendants()
     */
    function &FindSiblings($value, $property = 'id', $strict = false) {
        $ret = null;
        $cb = create_function('&$node, &$data',
            'if ($node->PropertyIsEqual($data[0], $data[1], $data[2])) { $data[3][] =& $node; }; return true;');
        $data = array($value, $property, $strict, &$ret);
        $this->WalkSiblings($cb, $data);

        return $ret;
    }

    /**
     * Call the given method on all sibling nodes.
     *
     * The nodes are called using a left to right traversal of the hierachy.
     *
     * @access  public
     * @param   string      $method     name of method to call
     * @param   array       $args       array of arguments to pass to method
     * @return  integer                 number of nodes called
     * @see     CallChildren(), CallAncestors(), CallDescendants()
     */
    function &CallSiblings($method, $args) {
        $ret = null;
        $cb = create_function('&$node, &$data',
            'call_user_func(array(&$node, $data[0], $data[1])); return true;');
        $data = array($method, $args);
        $ret = $this->WalkSiblings($cb, $data);

        return $ret;
    }

    /**
     * Returns an array of all siblings
     *
     * The nodes are returned using a left to right traversal of the hierachy.
     *
     * @access  public
     * @return  TNTNode[]               array of references to siblings
     * @see     Children(), Ancestors(), Descendants()
     */
    function &Siblings() {
        $ret = null;

        $cb = create_function('&$node, &$data',
            '$data[0][] =& $node; return true;');
        $data = array(&$ret);

        $this->WalkSiblings($cb, $data);

        return $ret;
    }

    /**
     * Returns the DN of a node
     *
     * The Distinguished Name is the heiracical path to the record from the
     * root node down. It consists of the values of the given member
     * concatinated together using the given separator.
     *
     * The DN consisting of just 'id' values is termed the nodes path.
     *
     * The values between separators are URL encoded.
     *
     * @access  public
     * @param   string  $property   name of member to display
     * @param   string  $sep        separator to place between values
     * @param   boolean $full       full name (ie $name=$val not just $val)
     * @param   boolean $incRoot    include the root objects name in the DN
     * @return  string              the DN of the node
     */
    function DN($property = 'id', $sep = '.', $full = false, $incRoot = true) {
        $ret = null;
        $nodes = array();
        if ($this->parent) {
            $nodes = array_reverse($this->Ancestors());
            if (!$incRoot) {
                array_shift($nodes);
            }
        }
        $nodes[] =& $this;

        $max_n = count($nodes);
        for ($n = 0; $n < $max_n; $n++) {
            $value = urlencode($nodes[$n]->GetMember($property));
            $ret[] = ($full ? ($property.'='.$value) : $value);
        }

        unset($nodes);

        if ($ret) {
            $ret = implode($sep, $ret);
        }

        return $ret;
    }

    /**
     * Returns the path of a node
     *
     * This is the distinguished name made up just of 'id' values separated by
     * periods.
     *
     * @access  public
     * @return  string              the path to the node
     */
    function Path() {
        return $this->DN();
    }

    /**
     * Return a node given its path relative to this node.
     *
     * @access  public
     * @param   string      $path       path of node to return
     * @return  return                  reference to correct node (or null)
     */
    function &NodeByPath($path)  {
        $ids = explode('.', $path);
        $ret =& $this;
        $name = '';
        $found = true;
        foreach ($ids as $id) {
            $name .= ($name ? ".$id" : $id);
            if (!($nodes = $ret->FindChildren($id, 'id'))) {
                $this->_SetError('Unable to find a node with id: '.$name);
                unset($ret);
                $ret = null;
                break;
            } else {
                $ret =& $nodes[0];
            }
        }

        return $ret;
    }

    /**
     * Construct the node
     *
     * This will construct node, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTNode() {

        TNTObject::TNTObject();

        // Construct the child collection
        $this->children =& $this->CreateChildCollection();
        $this->children->_owner =& $this;

        $this->parent = null;

    }

    /**
     * Destruct the object.
     *
     * This will destruct the node and any children.
     *
     * @access  private
     */
    function _TNTNode() {
        if ($this->parent) {
            $this->parent->children->Remove($this);
        }
        destroy($this->children);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
