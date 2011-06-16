///////////////////////////////////////////////////////////////////////////////
// TNT-JUIC: Javascript User Interface Components                    v2.0.01 //
// ==============================================                            //
//                                                                           //
// Copyright (c) 2003 by Nick Turner                                         //
// mail:info@tnt-juic.com                                                    //
// http://www.tnt-juic.com/                                                  //
//                                                                           //
// TNT-JUIC is an Javascript library of advanced user interface components   //
// for use on web based forms.                                               //
//                                                                           //
// WARNING: This package is protected by copyright law and international     //
// treaties. Unauthorized reproduction or distribution of this package,      //
// or any portion of it, may result in severe civil and criminal penalties,  //
// and will be prosecuted to the maximum extent possible under the law.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/**
 * TNT.Juic.Core.Nodes Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Generic base class for a collection of nodes.
 *
 * This object provides a set of methods to allow traversal of a collection
 * of nodes.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Juic.Core.Nodes = TNT.declareClass({
  $class : 'TNT.Juic.Core.Nodes',
  $extends : TNT.Juic.Core.Object,

  /**
   * Constructor
   *
   * @access  public
   * @return  object                  reference to constructed object
   */
  constructor : function() {
    TNT.Juic.Core.Object.prototype.constructor.call(this);

    this._nodes = [];
  },

  /**
   * The nodes in the collection
   * @access  private
   * @var     array                   collection of nodes
   */
  _nodes : null,

  /**
   * Return the number of nodes in the collection
   *
   * @access  public
   * @return  integer                 number of nodes
   */
  numNodes : function() {
    return this._nodes.length;
  },

  /**
   * Return an array of nodes in the collection
   *
   * @access  public
   * @return  array                   array of nodes
   */
  nodes : function() {
    return this._nodes;
  },


  /**
   * Return the index of a given node in the collection
   *
   * @access  public
   * @param   object    node          reference to node to return index of
   * @return  integer                 index of node (or -1)
   */
  indexOfNode : function(node) {
    var i, max_i = this._nodes.length;
    for (i = 0; i < max_i; i++) {
      if (this._nodes[i] == node) {
        return i;
      }
    }
    return -1;
  },

  /**
   * Return boolean if node is first in the collection
   *
   * @access  public
   * @param   object    node          reference to node to check
   * @return  boolean                 true if node is the first node
   */
  isFirstNode : function(node) {
    return (this._nodes.length > 0 && this._nodes[0] == node);
  },

  /**
   * Return boolean if node is last in the collection
   *
   * @access  public
   * @param   object    node          reference to node to check
   * @return  boolean                 true if node is the last node
   */
  isLastNode : function(node) {
    return (this._nodes.length > 0 && this._nodes[this._nodes.length - 1] == node);
  },

  /**
   * Reference to given node
   *
   * @access  public
   * @param   integer                 index of node to return
   * @return  object                  reference to the indexed node (or null)
   */
  node : function(idx) {
    return (idx >= 0 && idx < this._node.length ? this._nodes[idx] : null);
  },

  /**
   * Return the first node in the collection
   *
   * @access  public
   * @return  object                  reference to first node (or null)
   */
  firstNode : function() {
    return (this._nodes.length > 0 ? this._nodes[0] : null);
  },

  /**
   * Return the last node in the collection
   *
   * @access  public
   * @return  object                  reference to last node (or null)
   */
  lastNode : function() {
    return (this._nodes.length > 0 ? this._nodes[this._nodes.length - 1] : null);
  },

  /**
   * Return the next node to the specified node in the collection
   *
   * @access  public
   * @param   object    node          reference to node to get next one
   * @return  object                  reference to next node (or null)
   */
  nextNode : function(node) {
    var i = this.indexOfNode(node);
    return (i >= 0 && i <= (this._nodes.length - 2) ? this._nodes[i + 1] : null);
  },

  /**
   * Return the prev node to the specified node in the collection
   *
   * @access  public
   * @param   object    node          reference to node to get previous one
   * @return  object                  reference to previous node (or null)
   */
  prevNode : function(node) {
    var i = this.indexOfNode(node);
    return (i >= 1 ? this._nodes[i - 1] : null);
  },

  /**
   * Insert a node before the specified node (or at beginning if none specified)
   *
   * @access  public
   * @param   object    node          insert this node
   * @param   object    node2         insert before this node
   * @return  integer                 index where node was inserted (or -1)
   */
  insertNode : function(node, node2) {
    var idx = -1;
    if (node2) {
      idx = this.indexOfNode(node2);
      if (idx < 0) {
        return -1;
      }
      this._nodes.splice(idx, 0, node);
    } else {
      idx = 0;
      this._nodes.unshift(node);
    }
    return idx;
  },

  /**
   * Append a node after the specified node (or at end if none specified)
   *
   * @access  public
   * @param   object    node          append this node
   * @param   object    node2         append after this node
   * @return  integer                 index where node was appended (or -1)
   */
  appendNode : function(node, node2) {
    var idx = -1;
    if (node2) {
      idx = this.indexOfNode(node2) + 1;
      if (idx < 1) {
        return -1;
      }
      if (this._nodes.splice) {
        this._nodes.splice(idx, 0, node);
      } else {
        var _nodes = [];
        var i, max_i = this._nodes.length;
        for (i = 0; i < idx; i++) {
          _nodes[i] = this._nodes[i];
        }
        _nodex[idx] = node;
        for (i = idx; i < max_i; i++) {
          _nodes[i + 1] = this._nodes[i];
        }
        this._nodes = _nodes;
      }
    } else {
      idx = this._nodes.length + 1;
      this._nodes[idx] = node;
    }
    return idx;
  },

  /**
   * Remove a node at the given index.
   *
   * @access  public
   * @param   mixed     index|node    remove this node
   * @return  boolean                 true if node removed
   */
  removeNode : function(node) {
    var idx = (typeof node == 'number' ? parseInt(node) : this.indexOfNode(node));
    if (idx < 0) {
      return false;
    }
    this._nodes.splice(idx, 0, null);
    return true;
  },

  /**
   * Descend an event to all objects in the collection.
   *
   * @access  public
   * @param   string      name        event name or path
   * @param   mixed       event       event object
   * @return  void
   */
  descendEvent : function(name, event) {
    TNT.debug('descendEvent(', name, ', ', event, ')', this);

    TNT.Juic.Core.Object.prototype.raiseEvent.call(this, name, event);

    if (!event.cancelDescent && this._content) {
      var i, max_i = this._nodes.length();
      for (i = 0; i < max_i; i++) {
        if (this._nodes[i] && this._nodes[i] instanceof TNT.Juic.Core.Object) { 
          this._nodes[i].descendEvent(name, event);
        }
      }
    }
  },

  /**
   * Return the value of the object as a string.
   *
   * The method returns the concatenated string values of the nodes contents.
   *
   * @access  public
   * @return  string                  string representation of object
   */
  toString : function() {
    var str = '';
    var i, max_i = this._nodes.length;
    for (i = 0; i < max_i; i++) {
      str += this._nodes[i].toString();
    }
    return str;
  },

  /**
   * Taint the object.
   *
   * Calls taint on each node in the collection.
   *
   * @access  public
   * @param   object      event       event that caused the taint (optional)
   * @return  void
   */
  taint : function(event) {
    var i, max_i = this._nodes.length();
    for (i = 0; i < max_i; i++) {
      str += this._nodes[i].taint(event);
    }
  },


  /**
   * Destructor
   *
   * @access  private
   */
  _destructor : function() {
  }

});

/* vim: set expandtab tabstop=2 shiftwidth=2: */

