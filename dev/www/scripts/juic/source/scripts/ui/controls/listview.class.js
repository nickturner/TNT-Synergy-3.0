///////////////////////////////////////////////////////////////////////////////
// TNT-Juic: Javascript User Interface Components                    v2.0.01 //
// ==============================================                            //
//                                                                           //
// Copyright (c) 2003 by Nick Turner                                         //
// mail:info@tnt-juic.com                                                    //
// http://www.tnt-juic.com/                                                  //
//                                                                           //
// TNT-Juic is an Javascript library of advanced user interface components   //
// for use on web based forms.                                               //
//                                                                           //
// WARNING: This package is protected by copyright law and international     //
// treaties. Unauthorized reproduction or distribution of this package,      //
// or any portion of it, may result in severe civil and criminal penalties,  //
// and will be prosecuted to the maximum extent possible under the law.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/**
 * TNT.Juic.UI.Controls.ListView Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * A list view class emulates the Windows ListView object used by the Windows
 * Operating System. It is a powerfull and flexible control allowing the data
 * to be display in many forms based up a grid layout.
 *
 * @package TNT.Juic
 * @subpackage UI.Controls
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

if (!TNT.Juic.UI.Controls) {
  TNT.Juic.UI.Controls = {};
}

/**
 * TNT.Juic.UI.Controls.ListView Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Emulates a Windows list view control, which displays a collection of items
 * that can be displayed using different views.
 *
 * A ListView control allows you to display a list of items with item text and,
 * optionally, an icon to identify the type of item. For example, the Windows
 * Explorer list of files is similar in appearance to a ListView control. It
 * displays a list of the files and folders currently selected in the tree.
 * Each file and folder displays an icon associated with it to help identify
 * the type of file or folder. The ListViewItem class represents an item within
 * a ListView control. The items that are displayed in the list can be shown in
 * one of many different views. Items can be displayed as large icons, as small
 * icons, as tiles or thumbnails. Items can also have subitems which contain
 * information that is related to the parent item. The grid view, allows you to
 * display the item and its subitems in a grid with column headers that identify
 * the information being displayed in a subitem. ListView supports single or
 * multiple selection. The multiple selection feature lets the user select from
 * a list of items in a way similar to a ListBox control. Additionally, the user
 * can activate selected items to perform a task. For example, you could use a
 * ListView control to display a list of files that the application can then
 * open and utilize. The user can select the files to open and then double-click
 * them to activate the items and open the files in the application. The
 * ListView can also display check boxes, using the CheckBoxes property, to
 * allow the user to check the items that they want to perform an action on.
 * You can use the ListView control in a variety of ways. 
 *
 * The control provides a large number of properties that provide flexibility
 * in appearance and behavior. The View property allows you to change the way
 * in which items are displayed. Different Icons can be displayed in each view
 * and for each item state.
 *
 * If you want to allow the user to edit the text of an item, you can use the
 * LabelEdit property. When your control contains a large number of items, it
 * is often easier for the user to see them in a sorted list. You can use the
 * Sorting property and Sorter property to sort the items.
 *
 * Many of the properties of the ListView control are used when in Grid view.
 * The AllowColumnReorder property allows the user of your ListView control to
 * reconfigure the order of columns at run time. The FullRowSelect property
 * allows an item and its subitems to be selected instead of just the item. To
 * display grid lines in the details view to identify the boundaries of items
 * and subitems in the ListView, you can use the GridLines property.
 *
 * In addition to the many properties that are available for a ListView control,
 * there are methods and events that your application can use to provide
 * additional capabilities to the ListView. The BeginUpdate and EndUpdate
 * methods allow you to add many items to a ListView without displaying the
 * repainting of the control each time an item is added, improving performance.
 * The EnsureVisible method can be called to ensure that the specific item is
 * in the visible area of the control.
 *
 * @package TNT.Juic
 * @subpackage UI.Controls
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
TNT.Juic.UI.Controls.ListView = TNT.declareClass({
  $class : 'TNT.Juic.UI.Controls.ListView',
  $extends : TNT.Juic.UI.Control,

  /**
   * Constructor
   *
   * @access  public
   * @param   object    data          optional data map
   * @param   object    owner         optional owning Object
   * @return  object                  reference to constructed object
    */
  constructor : function(data, owner) {
    TNT.debug('ListView(', data, ',', owner, ')');

    TNT.Juic.UI.Control.prototype.constructor.call(this, data, owner);

    TNT.debug('ListView() - ', this);
  },

  /**
   * View to display
   * @access  private
   * @var     string                  view name
   */
  _view : 'icon',

  /**
   * Allow item labels to be edited
   * @access  private
   * @var     boolean                 true if labels are editable
   */
  _labelEdit : false,

  /**
   * Allow labels to wrap when displayed with icons
   * @access  private
   * @var     boolean                 true if labels can wrap
   */
  _labelWrap : false,

  /**
   * Allow multiple items to be selected
   * @access  private
   * @var     boolean                 true if multiple items can be selected
   */
  _multiSelect : false,

  /**
   * Allow columns to be re-ordered in GRID view
   * @access  private
   * @var     boolean                 true if column ordering is allowed
   */
  _columnReorder : false,

  /**
   * Display check boxes with the labels in GRID view
   * @access  private
   * @var     boolean                 true if check boxes are displayed
   */
  _checkBoxes : false,

  /**
   * Display grid lines in GRID view
   * @access  private
   * @var     boolean                 true if grid lines are displayed
   */
  _gridLines : false,

  /**
   * Highlight the whole selected row in GRID view as opposed to just label
   * @access  private
   * @var     boolean                 true if full row selection is enabled
   */
  _hoverSelection : false,

  /**
   * Automatically select the item if hovered over for a few seconds
   * @access  private
   * @var     boolean                 true if hover selection is enabled
   */
  _hoverSelection : null,

  /**
   * Sort the items before display
   * @access  private
   * @var     integer                 current SORT enumeration value
   */
  _sorting : 0,

  /**
   * Function that does the sort comparisons for the control
   * @access  private
   * @var     function                sort function
   */
  _sorter : null,

  /**
   * Array of indices of the items that are checked
   * @access  private
   * @var     array                   array of item indices
   */
  _checkedIndices : null,

  /**
   * Array of the items that are checked
   * @access  private
   * @var     array                   array of items
   */
  _checkedItems : null,

  /**
   * Array of indices of the items that are selected
   * @access  private
   * @var     array                   array of item indices
   */
  _selectedIndices : null,

  /**
   * Array of the items that are selected
   * @access  private
   * @var     array                   array of items
   */
  _selectedItems : null,

  /**
   * Item which has the current focus
   * @access  private
   * @var     object                  focused item
   */
  _focusedItem : null,

  // View enumeration
  VIEW : {
    ICON : 'icon', // A flow of large icons (32x32) with labels below
    MINI : 'mini', // A flow of small icons (16x16) with labels to the right
    LIST : 'list', // A list of small icons (16x16) with labels to the right
    TILE : 'tile', // A flow of large icons (32x32) with details to the right
    NAIL : 'nail', // A flow of thumbnail images () with labels befow
    GRID : 'grid'  // A grid of items, one item per line with columns
  },

  // Sort order enumeration
  SORT : {
    NONE : null,
    DESCENDING : 'desc',
    ASCENDING : 'asc'
  },

  /**
   * Set the current view
   *
   * This must be one of the views available in the {@link VIEW} enumeration.
   *
   * If a null value is specified then the first view in the list of available
   * views is used, this is normally 'icon' view.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getView();
   */ 
  setView : function(value) {
    var ret = false;

    if (this._view !== value) {
      this._view = value;
      this._changedProperty('view');
      this.refresh();
      ret = true;
    }

    return ret;
  },

  /**
   * Get the current view
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setView();
   */ 
  getView : function() {
    return this._view;
  },

  /**
   * Set whether the label can be edited.
   *
   * If set to true then double clicking on a label will allow its value to be
   * edited. The new value can be obtained from the corresponding item object.
   *
   * In addition to this an 'onlabeledit' event is raised.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getLabelEdit();
   */ 
  setLabelEdit : function(value) {
    var ret = false;
    if (this._labelEdit !== value) {
      this._labelEdit = value;
      this._changedProperty('labelEdit');
      ret = true;
    }

    return ret;
  },

  /**
   * Get whether the label can be edited.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setLabelEdit();
   */ 
  getLabelEdit : function() {
    return this._labelEdit;
  },

  /**
   * Set whether the label can wrap when displayed with its icon.
   *
   * If set to true then long labels will wrap.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getLabelWrap();
   */ 
  setLabelWrap : function(value) {
    var ret = false;
    if (this._labelWrap !== value) {
      this._labelWrap = value;
      this._changedProperty('labelWrap');
      ret = true;
    }

    return ret;
  },

  /**
   * Get whether the label can wrap when displayed with its icon.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setLabelWrap();
   */ 
  getLabelWrap : function() {
    return this._labelWrap;
  },

  /**
   * Set whether the mutiple items can be selected.
   *
   * If set to true then more than one item can be selected at once. To get
   * a collection of selected items use {@link Selected()}.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getMultiSelect();
   */ 
  setMultiSelect : function(value) {
    var ret = false;
    if (this._multiSelect !== value) {
      this._multiSelect = value;
      this._changedProperty('multiSelect');
      ret = true;
    }

    return ret;
  },

  /**
   * Get whether the mutiple items can be selected.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  boolean                 current value
   * @see     setMultiSelect();
   */ 
  getMultiSelect : function() {
    return TNT.toBoolean(this._multiSelect);
  },

  /**
   * Set whether the columns can be reordered in GRID view.
   *
   * If set to true then the user can drag column headers to reorder columns
   * in GRID view.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getColumnReorder();
   */ 
  setColumnReorder : function(value) {
    var ret = false;
    if (this._columnReorder !== value) {
      this._columnReorder = value;
      this._changedProperty('columnReorder');
      ret = true;
    }

    return ret;
  },

  /**
   * Get whether the columns can be reordered in GRID view.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setColumnReorder();
   */ 
  getColumnReorder : function() {
    return TNT.toBoolean(this._columnReorder);
  },

  /**
   * Set whether check boxes should be shown next to the label.
   *
   * The CheckBoxes property allows you to display a check box next to each
   * item in the list. This enables the user to select an item by clicking
   * the check box. The CheckBoxes property offers a way to select multiple
   * items in the ListView control without using the CTRL key. Using check
   * boxes to select items rather than the standard multiple selection method
   * may be easier. Even if the MultiSelect property of the ListView control
   * is set to false, you can still display checkboxes and provide multiple
   * selection capabilities to the user. This feature can be useful if you do
   * not want multiple items to be selected yet still want to allow the user
   * to choose multiple items from the list to perform an operation.
   *
   * To determine when an item has been checked, create an event handler for
   * the ItemCheck event. To get all the items that are checked use the
   * {@link CheckedItems()} method to access a collection of checked items.
   * To get the indexes of all items that are checked in the ListView, use
   * the {@link CheckedIndices()} method.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getCheckBoxes();
   */ 
  setCheckBoxes : function(value) {
    var ret = false;
    if (this._checkBoxes !== value) {
      // No change
      var v;
      if ((v = value) !== null && (v = TNT.toBoolean(value)) === null) {
        ret = this._invalidProperty('checkBoxes', value, 'ListView checkBoxes must be either null or a boolean value');
      } else {
        this._checkBoxes = value;
        this._changedProperty('checkBoxes');
        ret = true;
      }
    }

    return ret;
  },

  /**
   * Set whether check boxes should be shown next to the label.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setColumnReorder();
   */ 
  getCheckBoxes : function() {
    return TNT.toBoolean(this._checkBoxes);
  },

  /**
   * Set whether grid lines should be shown in GRID view.
   *
   * The GridLines property has no effect unless the control is in GRID view.
   * When in GRID view the property allows you to display lines to identify
   * the rows and columns that are displayed in the ListView control when it
   * displays items and their subitems. The grid lines that are displayed do
   * not provide the ability to resize rows and columns. Only columns can be
   * resized, if column headers are displayed, by moving the mouse pointer to
   * the right side of the column to resize and then clicking and dragging
   * until the column is the size you want. The grid lines feature is used to
   * provide the user of the control with visible boundaries around items and
   * subitems.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getGridLines();
   */ 
  setGridLines : function(value) {
    var ret = false;
    if (this._gridLines !== value) {
      this._gridLines = value;
      this._changedProperty('gridLines');
      ret = true;
    }

    return ret;
  },

  /**
   * Get whether grid lines should be shown in GRID view.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setGridLines();
   */ 
  getGridLines : function() {
    return this._gridLines;
  },

  /**
   * Set whether clicking an item selects all its subitems.
   *
   * The FullRowSelect property has no effect unless the control is in GRID
   * view. The property is typically used when a ListView displays items with
   * many subitems and it is important to be able to see selected items when
   * the item text is not visible due to horizontal scrolling of the control's
   * contents.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getFullRowSelect();
   */ 
  setFullRowSelect : function(value) {
    var ret = false;
    if (this._fullRowSelect !== value) {
      this._fullRowSelect = value;
      this._changedProperty('fullRowSelect');
      ret = true;
    }

    return ret;
  },

  /**
   * Get whether clicking an item selects all its subitems.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setFullRowSelect();
   */ 
  getFullRowSelect : function() {
    return this._fullRowSelect;
  },

  /**
   * Set whether an item is automatically selected when the mouse pointer
   * remains over the item for a few seconds.
   *
   * When this property is set to true, the user can point to an item in the
   * ListView control to select the item. Multiple items can be selected (when
   * the MultiSelect property is set to true) by holding down the CTRL key while
   * pointing to each item. You can use this feature to provide an easier method
   * for the user of your application to select items in the ListView control.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getHoverSelection();
   */ 
  setHoverSelection : function(value) {
    var ret = false;
    if (this._hoverSelection !== value) {
      this._hoverSelection = value;
      this._changedProperty('hoverSelection');
      ret = true;
    }

    return ret;
  },

  /**
   * Get whether an item is automatically selected when the mouse pointer
   * remains over the item for a few seconds.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setHoverSelection();
   */ 
  getHoverSelection : function() {
    return this._hoverSelection;
  },

  /**
   * Set the sort order for items in the control.
   *
   * The Sorting property allows you to specify whether or not items are sorted
   * in the ListView control. By default, no sorting is performed. When the
   * Sorting property is set to SORT.ASCENDING or SORT.DESCENDING, the items 
   * sorted automatically in ascending or descending alphabetical order. You
   * can use this property to automatically sort items that are displayed in
   * your ListView control to make it easier for users to find items when a
   * large number of items are available.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getSorting();
   */ 
  setSorting : function(value) {
    var ret = false;
    if (this._sorting !== value) {
      this._sorting = value;
      this._changedProperty('sorting');
      ret = true;
    }

    return ret;
  },

  /**
   * Get the sort order for items in the control.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setSorting();
   */ 
  getSorting : function() {
    return this._sorting;
  },

  /**
   * Set the columns from an array of ListViewColumn objects or an array
   * of hashes.
   *
   * Each columns is an associative array containing the following optional
   * members;
   *   + id       - integer: id (column 0 refers to item, 1+ to subitem)
   *   + align    - string: alignment ('left', 'right' or 'center')
   *   + width    - string: width in pixels or percentage
   *   + label    - string: header (HTML)
   *   + image    - string: image (either TNTIcon or src name)
   *   + imagePos - string: image position ('left' or 'right')
   *   + order    - integer: order (zero is leftmost)
   *
   * If 'columns' is set to a string value then it is assumed to be the
   * input to a data source object and be of the format 'type:data'.
   *
   * @access  public
   * @param   mixed       $value      value to set the columns to
   * @return  boolean                 true if changed, false if unchanged
   * @see     getColumns();
   */ 
  setColumns : function(value) {
    var ret = false;
    if (this._columns == value) {
      // No change
      ret = false;
    } else if (TNT.isNull(value)) {
      this._columns = null;
      ret = true;
    } else if (!TNT.isEnumerable(value)) {
      ret = this._invalidProperty('columns', value, 'ListView columns must be either null or an array of views');
    } else {
      this._columns = $value;
      ret = true;
    }
    return ret;
  },

  /**
   * Get the columns
   *
   * If there is no columns set then null is returned.
   *
   * This is an alias for {@link Columns()}.
   *
   * @access  public
   * @return  string                  the controls current value
   * @see     setColumns();
   */ 
  getColumns : function() {
    return this._columns;
  },

  /**
   * Get the columns
   *
   * If there is no columns set then null is returned.
   *
   * This is an alias for {@link Columns()}.
   *
   * @access  public
   * @return  string                  the controls current value
   * @see     getColumns();
   */ 
  Columns : function() {
    return this._columns;
  },

  /**
   * Set the columns from an array of ListViewColumn objects or an array
   * of hashes.
   *
   * This member specifies an array of items to be displayed by the list
   * control.
   *
   * Each items is an associative array containing the following optional
   * members;
   *   + id       - string: id
   *   + value    - string: the valye to store with this item
   *   + label    - string: label (HTML)
   *   + icon     - string: the path to the icon to display
   *   + tooltip  - string: tooltip
   *   + action   - string: the function to call or URL to goto when clicked
   *   + options  - array: an associative array of extended options
   *   + subItems - array: an array of sub items for columns
   *   + state    - integer: state (see below)
   *   + stateMask- integer: stateMask (which states are set above)
   *   + images   - array: images (per view and state)
   *   + indent   - integer: identation (in image widths)
   *   + userData - mixed: user data (string)
   *   + group    - string: group the item is in
   *   + tileCols - array: tile columns to display
   *   + subItems - array: sub items (array of sub items)
   *   + checked  - boolean: the checked state of any check box
   *   + class    - string: the a css class for the item
   *   + style    - string: the a css style for the item
   *   + events   - map: event handlers
   *
   * If 'items' is set to a string value then it is assumed to be the
   * input to a data source object and be of the format 'type:data'.
   *
   * If no 'label' is specified then one is made up by the control.
   *
   * If no 'value' is given then the 'label' is used. 
   *
   * The 'group' member can be used to indicate option grouping, different
   * list widgets will display groups in different ways.
   *
   * Each sub item is a map associative array containing the following
   * optional members;
   *   + id       - string: id
   *   + label    - string: label (HTML)
   *   + tooltip  - string: tooltip
   *   + events   - map: event handlers
   *   + class    - string: the a css class for the item
   *   + style    - string: the a css style for the item
   *   + action   - string: the function to call or URL to goto when clicked
   *   + options  - array: an associative array of extended options
   *
   * @access  public
   * @param   mixed       $value      value to set the items to
   * @return  boolean                 true if changed, false if unchanged
   * @see     getItems();
   */ 
  setItems : function(value) {
    var ret = false;
    if (this._items == value) {
      // No change
      ret = false;
    } else if (TNT.isNull(value)) {
      this._items = null;
      ret = true;
    } else if (!TNT.isEnumerable(value)) {
      ret = this._invalidProperty('items', value, 'ListView items must be either null or an array of views');
    } else {
      this._items = []; // value;
      // Ignore the value and fake some items
      for (i = 0; i < 300; i++) {
        this._items[i] = { 'label' : 'Item ' + i };
      }
      ret = true;
    }
    return ret;
  },

  /**
   * Get the items
   *
   * If there is no items set then null is returned.
   *
   * This is an alias for {@link Items()}.
   *
   * @access  public
   * @return  string                  the controls current value
   * @see     setItems();
   */ 
  getItems : function() {
    return this._items;
  },

  /**
   * Get the items
   *
   * Unlike {@link getItems()} this will create the item collection if not
   * yet created and not return null.
   *
   * @access  public
   * @return  string                  the controls current value
   * @see     getItems();
   */ 
  items : function() {
    if (!this._items) {
      this._items = new TNT.Juic.UI.Controls.ListView.ListViewItemCollection(this);
    }
    return this._items;
  },

  /**
   * Removes all items and columns from the control.
   *
   * You can use this method to remove all items and columns from the ListView
   * control without having to call the individual Clear methods from the
   * ListView.ColumnHeaderCollection and ListView.ListViewItemCollection
   * classes.
   *
   * @access  public
   * @return  void
   */ 
  clear : function(value) {
    this._items.clear();
    this._columns.clear();
  },

  /**
   * Ensures that the specified item is visible within the control, scrolling
   * the contents of the control if necessary.
   *
   * You can use this method to ensure that a specific item is visible within
   * the ListView control. If the item that you want to ensure is visible is
   * located above the viewable region of the ListView control, calling the
   * EnsureVisible method will scroll the contents of the control until it is
   * the first item in the viewable area of the control. If the item is below
   * the viewable region of the ListView control, calling the EnsureVisible
   * method will scroll the contents of the ListView control until the item is
   * the last item in the viewable area of the control.
   *
   * To determine if an item is located at the top of the display area of a
   * ListView control, use the TopItem property.
   *
   * @access  public
   * @param   integer     index       index of the item to scroll into view
   * @return  void
   */ 
  ensureVisble : function(index) {
  },

  /**
   * Sorts the items of the list view.
   *
   * This sorts the items as per the Sorting & Sorter properties.
   *
   * By default the Sorting property is NONE as in this case this method will
   * do nothing. To specify a sort order then set the Sorting property to either
   * SORT.ASCENDING or SORT.DESCENDING.
   *
   * By default the Sorter property is set to sort the items based on the value
   * property of each item, or the items label is the value property is not
   * defined.
   *
   * @access  public
   * @param   integer     index       index of the item to scroll into view
   * @return  void
   * @see     setSorting(), setSorter()
   */ 
  sort : function() {
  },

  /**
   * Render the items using flow layout.
   *
   * Renders the items so they flow accross the page wrapping at the right
   * margin of the containing box.
   *
   * @access  public
   * @param   string      view        the view name
   * @return  string                  html markup for the layout
   */ 
  _renderFlowLayout : function(view) {
    TNT.debug('ListView::_renderFlowLayout('+view+')');
    var st = TNT.microsecs();
  
    var html = '\
<div class="juic-listview-layout-flow" style="zoom: 1; padding: 1px;">';

    var items = this.items();

    for (var i = 0, l = items.count(); i < l; i++) {
      html += items.item(i);
    }

    html += '\
</div>';
    

    TNT.debug('ListView::_renderFlowLayout() - done in '+(TNT.microsecs() - st)+'ms');
    return html;
  },
  
  /**
   * Render the items using grid layout.
   *
   * Renders the items so they are in a grid showing the column information.
   *
   * @access  public
   * @param   string      view        the view name
   * @return  void
   */ 
  _renderGridLayout : function(view) {
  
    var id, item;
    var i, c, column, subItem;
  
    // Sort columns into correct order
    var colnum = 0;
    var columns = new Array();
    for (c in this._columns) {
      column = this._columns[c];
      if (columns[column.order]) {
        alert('Duplicate column order - column skipped');
      } else {
        columns[column.order] = column;
        colnum++;
      }
    }
  
    var pc = 0;
    if (colnum) {
      pc = (100 / colnum)+'%';
    }
    for (c in columns) {
      if (!columns[c].width) {
        columns[c].width = pc;
      }
    }
  
    var itemStyle = this.ItemStyle(view);
    var iconStyle = this.IconStyle(view);
    
    document.write('\
  <table cellpadding="0" cellspacing="0" border="0" width="100%" class="juic-listview-full">\
    <colgroup>\
      <col width="16" />\
      <col width="64" />\
    </colgroup>\
    <colgroup>');
  
    for (c in columns) {
      column = columns[c];
      document.write('\
        <col width="'+column.width+'" />');
    }
  
    document.write('\
    </colgroup>\
    <thead class="juic-listview-full">\
      <tr class="juic-listview-full">\
        <th class="juic-listview-full" colspan="2">Name</th>');
  
    for (c in columns) {
      column = columns[c];
      document.write('\
          <th class="juic-listview-full" align="center" valign="middle">'+column.label+'</div></th>');
    }
  
    document.write('\
      </tr>\
    </thead>\
    <tbody class="juic-listview-full">');
  
    var img = null;
  
    for (id in this._items) {
      var item = this._items[id];
      if (!img)
        imgsrc = this.ItemImage(item, view);
  
      document.write('\
      <tr id="'+item._id+'" class="juic-listview-item"\
        style="'+itemStyle.css+'"\
        onmouseover="return '+this._object+'._MouseOver(event,this);"\
        onmouseout="return '+this._object+'._MouseOut(event,this);"\
        onclick="return '+this._object+'._Click(event,this);"\
        ondblclick="return '+this._object+'._DblClick(event,this);"\
        oncontextmenu="return '+this._object+'._ContextMenu(event,this);">\
        <td class="juic-listview-icon" width="16"><div class="juic-listview-icon" style="'+iconStyle.css+'"><img id="'+item._id+'_img" '+img.attrs+' align="middle" alt="'+item.tooltip+'" /></div></td>\
        <td class="juic-listview-label" align="left" valign="middle" nowrap="nowrap" title="'+item.tooltip+'"><span class="juic-listview-label" style="white-space: nowrap"><nobr>'+item.label+'</nobr></span></td>');
  
      chtml = new Array();
      for (c in columns) {
        column = columns[c];
        subItem = item.subItems[column.id];
        document.write('\
          <td class="juic-listview-text" align="'+column.align+'" valign="middle" nowrap="nowrap"><span class="juic-listview-text" style="white-space: nowrap;"><nobr>'+subItem.label+'</nobr></span></td>');
      }
  
      document.write('\
      </tr>');
    }
  
    document.write('\
    </tbody>\
  </table>');
  },
  
  /**
   * Render the items in ICON view.
   *
   * This base method just calls {@link _renderFlowLayout()}. The purpose is
   * to allow derived classes to customize their own views.
   *
   * @access  public
   * @return  string                  html markup for the view
   */ 
  _renderIconView : function() {
    return this._renderFlowLayout(this.VIEW.ICON);
  },

  /**
   * Render the items in MINI view.
   *
   * This base method just calls {@link _renderFlowLayout()}. The purpose is
   * to allow derived classes to customize their own views.
   *
   * @access  public
   * @return  string                  html markup for the view
   */ 
  _renderMiniView : function() {
    return this._renderFlowLayout(this.VIEW.MINI);
  },

  /**
   * Render the items in LIST view.
   *
   * This base method just calls {@link _renderFlowLayout()}. The purpose is
   * to allow derived classes to customize their own views.
   *
   * @access  public
   * @return  string                  html markup for the view
   */ 
  _renderListView : function() {
    return this._renderFlowLayout(this.VIEW.LIST);
  },

  /**
   * Render the items in TILE view.
   *
   * This base method just calls {@link _renderFlowLayout()}. The purpose is
   * to allow derived classes to customize their own views.
   *
   * @access  public
   * @return  string                  html markup for the view
   */ 
  _renderTileView : function() {
    return this._renderFlowLayout(this.VIEW.TILE);
  },

  /**
   * Render the items in NAIL view.
   *
   * This base method just calls {@link _renderFlowLayout()}. The purpose is
   * to allow derived classes to customize their own views.
   *
   * @access  public
   * @return  string                  html markup for the view
   */ 
  _renderNailView : function() {
    return this._renderFlowLayout(this.VIEW.NAIL);
  },

  /**
   * Render the items in GRID view.
   *
   * This base method just calls {@link _renderGridLayout()}. The purpose is
   * to allow derived classes to customize their own views.
   *
   * @access  public
   * @return  string                  html markup for the view
   */ 
  _renderGridView : function() {
    return this._renderGridLayout(this.VIEW.GRID);
  },

/*
  ListView.prototype._GridMouseOver = function(event,elm) {
    var i = elm.id.split(':')[1];
    var item = this._items[i];
  
    var state = item.state;
    item.state |= ListView.states.hovered;
  
    if (item.state != state) {
      this.RefreshElement(elm, item);
    }
  
    if (item.onmouseover) {
      return eval(item.onmouseover);
    }
  
    return true;
  }
  
  ListView.prototype._GridMouseOut = function(event,elm) {
    var i = elm.id.split(':')[1];
    var item = this._items[i];
  
    var state = item.state;
    item.state &= (~ListView.states.hovered);
  
    if (item.state != state) {
      this.RefreshElement(elm, item);
    }
  
    if (item.onmouseout) {
      return eval(item.onmouseout);
    }
  
    return true;
  }
  
  ListView.prototype._GridClick = function(event,elm) {
    alert(elm);
    var i = elm.id.split(':')[1];
    var item = this._items[i];
  
    this.Select(item.id);
  
    if (item.onclick) {
      return eval(item.onclick);
    }
  
    return true;
  }
  
  ListView.prototype._GridDblClick = function(event,elm) {
    var i = elm.id.split(':')[1];
    var item = this._items[i];
  
    this.Select(item.id);
  
  
    if (item.ondblclick) {
      return eval(item.ondblclick);
    }
  
    return true;
  }
  
  ListView.prototype._GridContextMenu = function(event,elm) {
    var i = elm.id.split(':')[1];
    var item = this._items[i];
  
    this.Select(item.id);
  
    if (item.oncontextmenu) {
      return eval(item.oncontextmenu);
    }
  
    return true;
  }
*/
  
  /**
   * Render the named view.
   *
   * Renders the view using either a custom view method or layout method.
   *
   * @access  public
   * @param   string      view        the view name
   * @return  void
   */ 
  _renderView : function(view) {
    TNT.debug('ListView::_renderView('+view+')');
  

  
    TNT.debug('ListView::_renderView() - done');
    return html;
  },
  
  /**
   * Display the named view.
   *
   * Displays the view using either a custom view method or layout method.
   *
   * @access  public
   * @param   string      view        the view name
   * @param   boolean     display     true to display, false to hide
   * @return  void
   */ 
  _displayView : function(view, display) {
    TNT.debug('ListView::_displayView('+view+','+(display ? 'true' : 'false')+')');
  
    var ret = true;
  
    // To display a view we look for the following methods;
    // a) A view method 'Render<view>View()'
    // b) A layout method 'Render<layout>Layout()'
    // We then refresh the views items and display the views container
  
    if (this._views[view]) {
      var v = this._views[view].view;
      var func = '_Display'+v.substr(0,1).toUpperCase()+v.substr(1).toLowerCase()+'View';
      if (eval('this.'+func)) {
        ret = eval('this.'+func+'(\''+view+'\', '+(display ? 'true' : 'false')+');');
      } else {
        var n = this._views[view].layout || 'flow';
        var func = '_Render'+n.substr(0,1).toUpperCase()+n.substr(1).toLowerCase()+'Layout';
        if (eval('this.'+func)) {
          eval('this.'+func+'(\''+view+'\');');
        }
      }
    }
  
    // Refresh the elements to show the current state of the items
    var i, item;
    for (i in this.items) {
      this.RefreshElement(document.getElementById(this._getItemDomId(this.items[i], view)), this.items[i]);
    }
  
    // Display the division holding the view
    if (ret) {
      var id = this._id+'-view-'+view;
      var div = document.getElementById(id);
      if (div) {
        div.style.display = (display ? 'block' : 'none');
      } else {
        TNT.debug('View division can not be found'); 
      }
    } else {
      TNT.debug('View should not be displayed'); 
    }
  
    TNT.debug('ListView::_displayView()');
  },
  
  /**
   * Display the named view.
   *
   * Displays the view using either a custom view method or layout method.
   *
   * @access  public
   * @param   array       item        the item details
   * @param   string      view        the view name
   * @return  string                  dom id
   */ 
  _getItemDomId : function(item, view) {
    // As all views are rendered to the page together then to get a unique
    // item id for the document element for the view we do some jiggery pokery
    return this._id+'|'+view+'|'+item.id;
  },
  
  /**
   * Display the named view.
   *
   * Displays the view using either a custom view method or layout method.
   *
   * @access  public
   * @return  string      domId       dom id
   * @return  array                   the item details
   */ 
  _domIdToItem : function(domId) {
    var bits = domId.split('|', 3);
    if (!bits || bits.length != 3) {
      TNT.debug('Invalid Item DOM Id: '+domId);
      return null;
    }
    var id = bits[2];
    if (!this.items[id]) {
      TNT.debug('No item with id: '+id);
      return null;
    }
    return this.items[id];
  },
  
  /**
   * Display the control.
   *
   * @access  public
   * @return  void
   */ 
  render : function() {
    TNT.debug('ListView::render()');
  
    // If there is a cookie for use then get view from it.
    if ((view = TNT.getCookie(this.id))) {
      if (this._views[view]) {
        this.view = view;
      } else {
        alert('Unsupported view read from cookie: '+view);
      }
    }
  
    // Save the view in a cookie for later
    TNT.setCookie(this.id, this.view);
  
    var id;
    var cnt = 0;
  
    TNT.debug('Enumerating items');
    cnt = 0;
    for (id in this._items) {
      this._items[id].id = id;
      if (!this._items[id].state) this._items[id].state = 0;
      if (!this._items[id].image) this._items[id].image = '/synergy2/release/media/images/icons/32x32/access.gif';
      cnt++;
    }
    this._numItems = cnt;
    TNT.debug('There are '+cnt+' items');
  
    TNT.debug('Enumerating columns');
    cnt = 0;
    for (id in this._columns) {
      this._columns[id].id = id;
      cnt++;
    }
    this._numColumns = cnt;
    TNT.debug('There are '+cnt+' columns');
  
    TNT.debug('Enumerating views');
    cnt = 0;
    for (id in this._views) {
      this._views[id].id = id;
      cnt++;
    }
    this._numViews = cnt;
    TNT.debug('There are '+cnt+' views');
  
    document.write('\
  <style>\
  .juic-listview-layout-flow .juic-listview-item {\
    float: left;\
    font: icon; \
    overflow: hidden;\
    white-space: nowrap;\
    text-overflow: ellipsis;\
    color: white;\
  }\
  .juic-listview-layout-flow .juic-listview-icon {\
    font: icon; \
    overflow: hidden;\
  }\
  .juic-listview-layout-flow .juic-listview-name {\
    font: icon; \
    overflow: hidden;\
    text-overflow: ellipsis;\
    background-color: highlight;\
    color: white;\
  }\
  </style>');
  
    document.writeln('<div id="'+this._id+'" class="juic-listview">');
    document.writeln('<div id="'+this._id+'-view" class="juic-listview-view">');
  
    // Each view is output but not displayed
    var v;
    for (v in this._views) {
      this._RenderView(v);
    }
  
    document.writeln('</div><!-- class="juic-listview-view" -->');
    document.writeln('<br clear="both" />');
    document.writeln('<div class="juic-listview-form">');
    document.writeln('<form><select onChange="'+this._object+'.ChangeView(this.options[this.selectedIndex].value); return false;">');
    var v;
    for (v in this._views) {
      var selected = ((v == this.view) ? ' selected="selected"' : '');
      document.writeln('<option value="'+this.HTMLEscape(v)+'" '+selected+'>'+this._views[v].name+'</option>');
    }
    document.writeln('</select></form>');
    document.writeln('</div><!-- class="juic-listview-form" -->');
    document.writeln('</div><!-- class="juic-listview" -->');
  
    this._DisplayView(this.view, true);
  },
  
  /**
   * Display the control.
   *
   * @access  public
   * @return  string      string      the view name
   * @return  void
   */ 
  changeView : function(view) {
    if (view == this.view) {
      return;
    }
  
    // Hide all the view divisions
    this._DisplayView(this.view, false);
  
    this.view = view;
  
    // Show the new view division
    this._DisplayView(this.view, true);
  
    // Save the view in a cookie for later
    TNT.setCookie(this.id, this.view);
  },
  
  /**
   * Refresh the DOM element.
   *
   * @access  public
   * @return  object      elm         DOM element
   * @return  array       item        item details
   * @return  void
   */ 
  refreshElement : function(elm, item) {
  /*
    if (elm) {
      var className = elm.className.replace(/juic-listview-item([^ ]*)/, '');
      TNT.debug('className='+className);
      elm.className = this.ItemClassName(item) + ' ' + className;
    } 
  */
  },
  
  /**
   * Select the given item.
   *
   * @access  public
   * @return  string      id          item id
   * @return  void
   */ 
  select : function(id) {
    // unselect all items and select this one
    var i, item;
    for (i in this.items) {
      item = this.items[i];
      if (item.id == id) {
        // Select this item
        if (!(item.state & ListView.states.selected)) {
          item.state |= ListView.states.selected;
          this.RefreshElement(document.getElementById(this._getItemDomId(item, this.view)), item);
        }
      } else {
        // Unselect this item
        if (item.state & ListView.states.selected) {
          item.state &= (~ListView.states.selected);
          this.RefreshElement(document.getElementById(this._getItemDomId(item, this.view)), item);
        }
      }
    }
  },
  
  /**
   * Select the given item.
   *
   * @access  public
   * @return  string      id          item id
   * @return  void
   */ 
/*
  ListView.prototype._MouseOver = function(event,elm) {
    var item = this.DomIdToItem(elm.id);
  
    var state = item.state;
    item.state |= ListView.states.hovered;
  
    if (item.state != state) {
      this.RefreshElement(elm, item);
    }
  
    if (item.onmouseover) {
      return eval(item.onmouseover);
    }
  
    return true;
  }
  
  ListView.prototype._MouseOut = function(event,elm) {
    var item = this.DomIdToItem(elm.id);
  
    var state = item.state;
    item.state &= (~ListView.states.hovered);
  
    if (item.state != state) {
      this.RefreshElement(elm, item);
    }
  
    if (item.onmouseout) {
      return eval(item.onmouseout);
    }
    return true;
  }
  
  
  ListView.prototype._Click = function(event,elm) {
    var item = this.DomIdToItem(elm.id);
  
    this.Select(item.id);
  
    if (item.onclick) {
      return eval(item.onclick);
    }
  
    return true;
  }
  
  ListView.prototype._DblClick = function(event,elm) {
    var item = this.DomIdToItem(elm.id);
  
    this.Select(item.id);
  
    if (item.ondblclick) {
      return eval(item.ondblclick);
    }
  
    return true;
  }
  
  ListView.prototype._ContextMenu = function(event,elm) {
    var item = this.DomIdToItem(elm.id);
  
    this.Select(item.id);
  
    if (item.oncontextmenu) {
      return eval(item.oncontextmenu);
    }
  
    return true;
  }
*/

  /**
   * Perform the default action when the item is clicked.
   *
   * By defaul the innerHTML is the concatenated markup of the contents as
   * returned by {@link getContent()} when called with no arguments.
   *
   * @access  public
   * @param   boolean     refresh     force refresh of any cached values
   * @return  string                  inner HTML
   */
  _itemClick : function(event, item) {
    TNT.debug(this.className()+'::_itemClick()' + item.getName());
    item._selected = !item._selected;
    item.setState('selected', item._selected);
    return true;
  },

  /**
   * Return the HTML markup required to render the objects contents.
   *
   * This returns a correctly formatted HTML markup string required to render
   * the objects contents in the browser.
   *
   * @access  public
   * @return  string                  HTML markup for the object
   */
  innerHTML : function() {
    var view = this.getView();
    var renderer = '_render'+view.ucfirst()+'View';

    if (!this[renderer]) {
      throw 'TNT.Juic.UI.Controls.ListView() - no method to render view: '+view;
      return null;
    }

    var html = '\
      <!-- Start of View "'+view+'"-->\
      <div class="juic-listview-view-'+view+'" style="border: 1px solid red; background-color: white;">\
      ' + this[renderer]() + '\
      <br clear="both" />\
      </div>\
      <!-- End of View "'+view+'"-->\
      ';

    return html;
  },

  /**
   * Destructor
   *
   * @access  public
   * @return  void
   */
  _destructor : function() {
  }

});

TNT.Juic.UI.Controls.ListViewItem = TNT.declareClass({
  $class : 'TNT.Juic.UI.Controls.ListViewItem',
  $extends : TNT.Juic.Core.HTML,

  /**
   * Constructor
   *
   * @access  public
   * @return  object                  reference to constructed object
   */
  constructor : function(data) {
    TNT.Juic.Core.HTML.prototype.constructor.call(this);
    this._data = data;
  },

  _data : null,

  toString : function() {
    var name = this._data.name;
    var icon = this._data.icon;
    var tooltip = this._data.tooltip;

    var id = this.getId();

    var html = '\
  <!-- ListViewItem -->\
  <div id="'+id+'" class="juic-listviewitem" juic=""\
    title="'+tooltip+'">\
    <span class="juic-listviewitem-icon" juic="">\
      <img id="'+id+'_img" src="'+icon+'" />\
    </span>\
    <span class="juic-listviewitem-name" juic="">\
      '+name+'\
    </span>\
  </div>\
  <!-- /ListViewItem -->';

    return html;
  },

  /**
   * Destructor
   *
   * @access  public
   * @return  void
   */
  _destructor : function() {
  }

});

/**
 * TNT.Juic.UI.Controls.ListView.ListViewItemCollection Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents the collection of items in a ListView control.
 * 
 * A ListViewItemCollection stores the items displayed in the ListView control.
 *
 * To add items, you can use either the {@link add()} method or the
 * {@addAt()} if you want to add items at a specific location in the
 * collection. To add multiple items use the {@link addMany()} method.
 *
 * To remove items, you can use either the {@link remove()} method or the
 * {@link removeAt()} if you want to remove an items at a specific location
 * in the collection. To remove multiple items use the {@link removeMany()}
 * methid. The {@link clear()} method will remove all items.
 *
 * In addition to methods and properties for adding and removing items, the
 * Collection also provides methods to find items. The {@link contains()}
 * method enables you to determine whether an items is in the collection. The
 * {@link indexOf()} method will tell you where the items is located in the
 * collection.
 *
 * @package TNT.Juic
 * @subpackage UI.Controls
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
TNT.Juic.UI.Controls.ListView.ListViewItemCollection = TNT.declareClass({
  $class : 'TNT.Juic.UI.Controls.ListView.ListViewItemCollection',
  $extends : TNT.Juic.Core.ObjectCollection,

  /**
   * Constructor
   *
   * @access  public
   * @param   object      owner       Object that owns the collection
   * @return  object                  reference to constructed object
   */
  constructor : function(owner) {
    TNT.Juic.Core.ObjectCollection.prototype.constructor.call(this, owner, TNT.Juic.UI.Controls.ListView.ListViewItem);
  },

  /**
   * Prevent the items from being sorted.
   * @access  private
   * @var     boolean                 true if items should not be sorted
   */
  _sort : true,

  /**
   * Adds an item to the collection.
   *
   * If the method is called with a single argument which is a ListViewItem
   * object then that item is added.
   *
   * Otherwise a new ListViewItem object is created and the arguments are
   * passed to the objects constructor to initialise the item. The new item
   * is then added.
   *
   * To add multiple items use the {@link addMany()} method.
   * To add an items at a specific index use the {@link addAt()} method.
   *
   * The method returns the item added.
   *
   * If the ListView is sorted as determined by its Sorting property then
   * the item is added in the correct sorted position. If multiple items
   * are to be added the use {@link addMany()} which will only resort the
   * items after all have been added.
   *
   * @access  public
   * @param   mixed       item        item to add
   * @return  object                  item added
   * @see     addMany(), addAt()
   */
  add : function(item) {
    var ret = TNT.Juic.Core.ObjectCollection.prototype.add.call(this, item);

    if (ret && this._sort) {
      this._owner$.sort();
    }
   
    return ret;
  },

  /**
   * Adds multiple items to the collection.
   *
   * This method just calls the {@link add()} method for each value in the
   * specified array. However sorting is not performed until all are added.
   *
   * To add a single item use the {@link add()} method.
   * To add an item at a specific index use the {@link addAt()} method.
   *
   * The method returns an array of the item added.
   *
   * If the ListView is sorted as determined by its Sorting property then
   * the items are sorted after all are added.
   *
   * @access  public
   * @param   array       a           array of items to add
   * @return  array                   array of items added
   * @see     add(), addAt()
   */
  addMany : function(a) {
    this._sort = false;
    var ret = TNT.Juic.Core.ObjectCollection.prototype.addMany.call(this, a);
    this._sort = true;
    return ret;
  },

  /**
   * Destructor
   *
   * @access  public
   * @return  void
   */
  _destructor : function() {
  }

});

/**
 * TNT.Juic.UI.Controls.ListView.ListViewItem Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents an item in a ListView control.
 * 
 * The ListView control is similar to a ListBox in that it displays a list of
 * items. The main difference is that the ListView control provides a number of
 * different ways items can be viewed by the user. The ListViewItem class
 * defines the appearance, behavior, and data associated with an item that is
 * displayed in the ListView control. ListViewItem objects can be displayed in
 * the ListView control in different views. Items can be displayed as large or
 * small icons or as thumbnails. Items can also have subitems that contain
 * information that is related to the parent item. The grid view, allows you
 * to display the item and its subitems in a grid with column headers that can
 * be used to identify the information being displayed in a subitem.
 *
 * Most of the properties of the ListViewItem class provide ways to change the
 * display of the item in the ListView control it is associated with. The items
 * ClassName and Style properties allow you to change how the is displayed in
 * the ListView control. The Image property allows you to specify the image to
 * display in each view. Items can display check boxes in order to obtain item
 * choices from the user in a way similar to a CheckedListBox control. You can
 * use the Checked property to determine if an item is checked or to check or
 * uncheck the item at run time. Items can display any number of subitems when
 * the View property of the associated ListView control is set to VIEW.GRID and
 * columns are defined in the Columns property ListView control. You can add
 * subitems to an item by calling adding to the SubItems Array property.
 *
 * Some of the properties and methods of the class are item-specific versions
 * of properties and methods in the ListView control. For example, the 
 * {@link ensureVisible()} method is similar to the ListView version of the
 * method, but the ListViewItem version only affects the current item.
 *
 * The class also provides methods that are not versions of ListView methods.
 * The {@link beginEdit()} method places the item's text into edit mode so the
 * user can change the item's text (when the LabelEdit property of the parent
 * ListView control is set to true).
 *
 * @package TNT.Juic
 * @subpackage UI.Controls
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
TNT.Juic.UI.Controls.ListView.ListViewItem = TNT.declareClass({
  $class : 'TNT.Juic.UI.Controls.ListView.ListViewItem',
  $extends : TNT.Juic.Core.Object,

  /**
   * Constructor
   *
   * The constructor can either be called with a list of arguments as
   * documented or with a single Object argument which is used as an
   * hash of properties to initialise the item with.
   *
   * @access  public
   * @param   string      text        HTML to display as items label (optional)
   * @param   mixed       icon        icon to display or icon hash (optional)
   * @param   mixed       data        user data to store with item (optional)
   * @return  object                  reference to constructed object
   */
  constructor : function(text, icon, data) {
    TNT.Juic.Core.Object.prototype.constructor.call(this);

    if (arguments.length == 1 && name instanceof Object) {
      this.set(text);
    } else {
      if (text !== undefined) {
        this.setText(text);
      }
      if (icon !== undefined) {
        this.setIcon(icon);
      }
      /*
      if (data !== undefined) {
        this.setUserData(data);
      }
      */
    }
  },

  // State enumeration
  STATE : {
    HOVERED : 0x0001,
    FOCUSED : 0x0002,
    SELECTED : 0x0003,
    CHECKED : 0x0004 },

  /**
   * Gets or sets the text of the item.
   *
   * The Text property allows you to change the text displayed for the item.
   *
   * The ListView class provides the LabelWrap property that determines whether
   * text wraps to the next line or is displayed on a single line.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getText();
   */ 
  setText : function(value) {
    var ret = false;
    if (this._text !== value) {
      // No change
      if (value !== null && !TNT.isString(value)) {
        ret = this._invalidProperty('text', value, 'ListViewItem text must be either null or a string');
      } else {
        this._text = value;
        this._changedProperty('text');
        ret = true;
      }
    }

    return ret;
  },

  /**
   * Gets or sets the text of the item.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setText();
   */ 
  getText : function() {
    return this._text;
  },

  /**
   * Gets or sets the icon of the item.
   *
   * The Icon property allows you to change the icon displayed for the item.
   *
   * The value can either be a string or an Object hash of {'view' : 'icon'}
   * pairs to specify a different icon for each view. Further more the value
   * in the hash can specify an icon for the different states of the object.
   *
   * For example,
   *   + setIcon('file.png')
   *     - this will use 'file.png' for all states in all views.
   *
   *   + setIIcon({'icon' : 'file-32.png', 'mini' : 'file-16.png'})
   *     - this will use 'file-32.png' for large icons and 'file-16.png' for
   *       small icons.
   *
   *   + setIIcon({'icon' : 'file.png', 'icon-selected' : 'fileS.png'})
   *     - this will use 'file.png' for all icons and 'fileS.png' for all
   *       icons on selected items.
   *
   *   + setIIcon({'icon' : 'file.png', 'icon-selected' : 'fileS.png'})
   *     - this will use 'file.png' for all icons and 'fileS.png' for all
   *       icons on selected items.
   *
   * The indices of the hash are formatted as follows;
   *   <view>[-<state>][...-<state>]
   * where <view> is the current view name and <state> is the CSS psuedo state
   * string applied to the item.
   * 
   * If an hash is specified but doesn't include all views then a view will
   * use the closest icon for that view. For example, if the hash contains
   * icons for ICON and MINI views then the LIST and GRID views will use the
   * MINI views icon and the TILE and NAIL views will use the ICON views icon.
   *
   * Note the icon size is ignored, any icon is resized by the browser the the
   * size required.
   *
   * @access  public
   * @param   mixed       value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getText();
   */ 
  setIcon : function(value) {
    var ret = false;
    if (this._text !== value) {
      this._text = value;
      this._changedProperty('icon');
      ret = true;
    }

    return ret;
  },

  /**
   * Gets or sets the icon of the item.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setText();
   */ 
  getIcon : function() {
    return this._icon;
  },

  _checked : false,

  _focused : false,

  _selected : false,

  _subItems : null,

  /**
   * Gets the zero-based index of the item within the list view control.
   *
   * You can use this method to determine if the item is associated with a
   * ListView control as well as to determine its position within the items
   * of that control.
   *
   * If the item is not associated with a ListView control, this method
   * returns -1.
   *
   * @access  public
   * @return  integer                 index in parent items (else -1)
   */
  index : function() {
    var owner = this.owner();
    return (owner ? owner.items().indexOf(this) : -1);
  },

  /**
   * Places the item text into edit mode.
   *
   * This method is only effective if the LabelEdit property of the ListView
   * control that contains the item is set to true. You can use this method at
   * run time to force the item's text to display in edit mode. For example, if
   * you are validating the item text edited by the user, and an item fails
   * validation, you could select the item in the ListView control and call
   * the BeginEdit method to force the user to change the text that failed
   * validation.
   *
   * @access  public
   * @return  boolean                 true if edit mode started
   */
  beginEdit : function() {
  },

  /**
   * Ensures that the item is visible within the control, scrolling the
   * contents of the control if necessary.
   * 
   * You can use this method to ensure that the item is visible within the
   * ListView control. This method can be used when performing validation on
   * the item. You can call the EnsureVisible method to ensure that the item
   * is displayed in the ListView control, if it failed validation, to allow
   * the user to perform changes on the item.
   *
   * This method is a shortcut to calling the ensureVisible() method of the
   * owning ListView control with the index of this item.
   *
   * @access  public
   * @return  void
   */
  ensureVisible : function() {
    var owner = this.owner();
    if (owner) {
      owner.ensureVisible(owner.items().indexOf(this));
    }
  },

  /**
   * Removes the item from its associated list view control.
   * 
   * You can use this method to remove an item from its ListView control.
   *
   * The method is a short cut to the remove() method on the parent ListView
   * controls item collection.
   *
   * @access  public
   * @return  void
   */
  remove : function() {
    var owner = this.owner();
    if (owner) {
      owner.items().remove(this);
    }
  },

  /**
   * Return the HTML markup required to render the object.
   *
   * This returns the markup required to render the object in Flow mode.
   *
   * @access  public
   * @return  string                  HTML markup for the object
   */
  _renderFlowItem : function() {
    var i = this.index();
    var id = 'item#'+i;
    var name = 'Item '+i+' with a really long awkward name just to test the code';
    var icon = JUIC_DIR + '../../../media/images/icons/32x32/access.gif';
    var tooltip = 'Click me';

    var html = '\
      <!-- ListViewItem -->\
      <div id="'+id+'" class="juic-listviewitem" juic="'+id+'"\
        title="'+tooltip+'">\
        <span class="juic-listviewitem-icon">\
          <span class="juic-listviewitem-icon-inner">\
            <img id="'+id+'_img" src="'+icon+'" />\
          </span>\
        </span>\
        <span class="juic-listviewitem-name">\
          <span class="juic-listviewitem-name-inner">\
            '+name+'\
          </span>\
        </span>\
      </div>\
      <!-- /ListViewItem -->';

    return html;
  },

  /**
   * Return the HTML markup required to render the object.
   *
   * This returns the markup required to render the object in Grid mode.
   *
   * @access  public
   * @return  string                  HTML markup for the object
   */
  _renderGridItem : function() {
  },

  /**
   * Return the HTML markup required to render the object.
   *
   * This returns a correctly formatted HTML markup string required to render
   * the ListViewItem in the browser.
   *
   * @access  public
   * @return  string                  HTML markup for the object
   */
  toString : function() {
    if (this._listview.view == this._listview.VIEW.GRID) {
      return this._renderGridView();
    } else {
      return this._renderFlowView();
    }
  },

  /**
   * Get the correct icon url for the current item
   *
   * Checks the items state etc. to return the correct image.
   *
   * @access  public
   * @return  string                  the items icon url
   */ 
  _itemIcon : function(item, view) {
  },

  /**
   * Destructor
   *
   * @access  public
   * @return  void
   */
  _destructor : function() {
  }

});

/**
 * TNT.Juic.UI.Controls.ListView.ListViewColumnCollection Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents the collection of columns in a ListView control.
 * 
 * A ListViewColumnCollection stores the columns displayed in the ListView control.
 *
 * To add columns, you can use either the {@link add()} method or the
 * {@addAt()} if you want to add columns at a specific location in the
 * collection. To add multiple columns use the {@link addMany()} method.
 *
 * To remove columns, you can use either the {@link remove()} method or the
 * {@link removeAt()} if you want to remove an columns at a specific location
 * in the collection. To remove multiple columns use the {@link removeMany()}
 * methid. The {@link clear()} method will remove all columns.
 *
 * In addition to methods and properties for adding and removing columns, the
 * Collection also provides methods to find columns. The {@link contains()}
 * method enables you to determine whether an columns is in the collection. The
 * {@link indexOf()} method will tell you where the columns is located in the
 * collection.
 *
 * @package TNT.Juic
 * @subpackage UI.Controls
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
TNT.Juic.UI.Controls.ListView.ListViewColumnCollection = TNT.declareClass({
  $class : 'TNT.Juic.UI.Controls.ListView.ListViewColumnCollection',
  $extends : TNT.Juic.Core.ObjectCollection,

  /**
   * Constructor
   *
   * @access  public
   * @param   object      owner       Object that owns the collection
   * @return  object                  reference to constructed object
   */
  constructor : function(owner) {
    TNT.Juic.Core.ObjectCollection.prototype.constructor.call(this, owner, TNT.Juic.UI.Controls.ListView.ListViewColumn);
  },

  /**
   * Prevent the columns from being sorted.
   * @access  private
   * @var     boolean                 true if columns should not be sorted
   */
  _sort : true,

  /**
   * Adds an column to the collection.
   *
   * If the method is called with a single argument which is a ListViewColumn
   * object then that column is added.
   *
   * Otherwise a new ListViewColumn object is created and the arguments are
   * passed to the objects constructor to initialise the column. The new column
   * is then added.
   *
   * To add multiple columns use the {@link addMany()} method.
   * To add an columns at a specific index use the {@link addAt()} method.
   *
   * The method returns the column added.
   *
   * If the ListView is sorted as determined by its Sorting property then
   * the column is added in the correct sorted position. If multiple columns
   * are to be added the use {@link addMany()} which will only resort the
   * columns after all have been added.
   *
   * @access  public
   * @param   mixed       column      column to add
   * @return  object                  column added
   * @see     addMany(), addAt()
   */
  add : function(column) {
    var ret = TNT.Juic.Core.ObjectCollection.prototype.add.call(this, column);

    if (ret && this._sort) {
      this._owner$.sort();
    }
   
    return ret;
  },

  /**
   * Adds multiple columns to the collection.
   *
   * This method just calls the {@link add()} method for each value in the
   * specified array. However sorting is not performed until all are added.
   *
   * To add a single column use the {@link add()} method.
   * To add an column at a specific index use the {@link addAt()} method.
   *
   * The method returns an array of the column added.
   *
   * If the ListView is sorted as determined by its Sorting property then
   * the columns are sorted after all are added.
   *
   * @access  public
   * @param   array       a           array of columns to add
   * @return  array                   array of columns added
   * @see     add(), addAt()
   */
  addMany : function(a) {
    this._sort = false;
    var ret = TNT.Juic.Core.ObjectCollection.prototype.addMany.call(this, a);
    this._sort = true;
    return ret;
  },

  /**
   * Destructor
   *
   * @access  public
   * @return  void
   */
  _destructor : function() {
  }

});

/**
 * TNT.Juic.UI.Controls.ListView.ListViewColumn Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents an column in a ListView control.
 * 
 * The ListView control is similar to a ListBox in that it displays a list of
 * columns. The main difference is that the ListView control provides a number of
 * different ways columns can be viewed by the user. The ListViewColumn class
 * defines the appearance, behavior, and data associated with an column that is
 * displayed in the ListView control. ListViewColumn objects can be displayed in
 * the ListView control in different views. Columns can be displayed as large or
 * small icons or as thumbnails. Columns can also have subcolumns that contain
 * information that is related to the parent column. The grid view, allows you
 * to display the column and its subcolumns in a grid with column headers that can
 * be used to identify the information being displayed in a subcolumn.
 *
 * Most of the properties of the ListViewColumn class provide ways to change the
 * display of the column in the ListView control it is associated with. The columns
 * ClassName and Style properties allow you to change how the is displayed in
 * the ListView control. The Image property allows you to specify the image to
 * display in each view. Columns can display check boxes in order to obtain column
 * choices from the user in a way similar to a CheckedListBox control. You can
 * use the Checked property to determine if an column is checked or to check or
 * uncheck the column at run time. Columns can display any number of subcolumns when
 * the View property of the associated ListView control is set to VIEW.GRID and
 * columns are defined in the Columns property ListView control. You can add
 * subcolumns to an column by calling adding to the SubColumns Array property.
 *
 * Some of the properties and methods of the class are column-specific versions
 * of properties and methods in the ListView control. For example, the 
 * {@link ensureVisible()} method is similar to the ListView version of the
 * method, but the ListViewColumn version only affects the current column.
 *
 * The class also provides methods that are not versions of ListView methods.
 * The {@link beginEdit()} method places the column's text into edit mode so the
 * user can change the column's text (when the LabelEdit property of the parent
 * ListView control is set to true).
 *
 * @package TNT.Juic
 * @subpackage UI.Controls
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
TNT.Juic.UI.Controls.ListView.ListViewColumn = TNT.declareClass({
  $class : 'TNT.Juic.UI.Controls.ListView.ListViewColumn',
  $extends : TNT.Juic.Core.Object,

  /**
   * Constructor
   *
   * The constructor can either be called with a list of arguments as
   * documented or with a single Object argument which is used as an
   * hash of properties to initialise the column with.
   *
   * @access  public
   * @param   string      text        HTML to display as columns label (optional)
   * @param   mixed       icon        icon to display or icon hash (optional)
   * @param   mixed       data        user data to store with column (optional)
   * @return  object                  reference to constructed object
   */
  constructor : function(text, icon, data) {
    TNT.Juic.Core.Object.prototype.constructor.call(this);

    if (arguments.length == 1 && name instanceof Object) {
      this.set(text);
    } else {
      if (text !== undefined) {
        this.setText(text);
      }
      if (icon !== undefined) {
        this.setIcon(icon);
      }
      /*
      if (data !== undefined) {
        this.setUserData(data);
      }
      */
    }
  },

  // State enumeration
  STATE : {
    HOVERED : 0x0001,
    FOCUSED : 0x0002,
    SELECTED : 0x0003,
    CHECKED : 0x0004 },

  /**
   * Gets or sets the text of the column.
   *
   * The Text property allows you to change the text displayed for the column.
   *
   * The ListView class provides the LabelWrap property that determines whether
   * text wraps to the next line or is displayed on a single line.
   *
   * @access  public
   * @param   string      value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getText();
   */ 
  setText : function(value) {
    var ret = false;
    if (this._text !== value) {
      // No change
      if (value !== null && !TNT.isString(value)) {
        ret = this._invalidProperty('text', value, 'ListViewColumn text must be either null or a string');
      } else {
        this._text = value;
        this._changedProperty('text');
        ret = true;
      }
    }

    return ret;
  },

  /**
   * Gets or sets the text of the column.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setText();
   */ 
  getText : function() {
    return this._text;
  },

  /**
   * Gets or sets the icon of the column.
   *
   * The Icon property allows you to change the icon displayed for the column.
   *
   * The value can either be a string or an Object hash of {'view' : 'icon'}
   * pairs to specify a different icon for each view. Further more the value
   * in the hash can specify an icon for the different states of the object.
   *
   * For example,
   *   + setIcon('file.png')
   *     - this will use 'file.png' for all states in all views.
   *
   *   + setIIcon({'icon' : 'file-32.png', 'mini' : 'file-16.png'})
   *     - this will use 'file-32.png' for large icons and 'file-16.png' for
   *       small icons.
   *
   *   + setIIcon({'icon' : 'file.png', 'icon-selected' : 'fileS.png'})
   *     - this will use 'file.png' for all icons and 'fileS.png' for all
   *       icons on selected columns.
   *
   *   + setIIcon({'icon' : 'file.png', 'icon-selected' : 'fileS.png'})
   *     - this will use 'file.png' for all icons and 'fileS.png' for all
   *       icons on selected columns.
   *
   * The indices of the hash are formatted as follows;
   *   <view>[-<state>][...-<state>]
   * where <view> is the current view name and <state> is the CSS psuedo state
   * string applied to the column.
   * 
   * If an hash is specified but doesn't include all views then a view will
   * use the closest icon for that view. For example, if the hash contains
   * icons for ICON and MINI views then the LIST and GRID views will use the
   * MINI views icon and the TILE and NAIL views will use the ICON views icon.
   *
   * Note the icon size is ignored, any icon is resized by the browser the the
   * size required.
   *
   * @access  public
   * @param   mixed       value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getText();
   */ 
  setIcon : function(value) {
    var ret = false;
    if (this._text !== value) {
      this._text = value;
      this._changedProperty('icon');
      ret = true;
    }

    return ret;
  },

  /**
   * Gets or sets the icon of the column.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string                  current value
   * @see     setText();
   */ 
  getIcon : function() {
    return this._icon;
  },

  _checked : false,

  _focused : false,

  _selected : false,

  _subColumns : null,

  /**
   * Gets the zero-based index of the column within the list view control.
   *
   * You can use this method to determine if the column is associated with a
   * ListView control as well as to determine its position within the columns
   * of that control.
   *
   * If the column is not associated with a ListView control, this method
   * returns -1.
   *
   * @access  public
   * @return  integer                 index in parent columns (else -1)
   */
  index : function() {
    var owner = this.owner();
    return (owner ? owner.columns().indexOf(this) : -1);
  },

  /**
   * Places the column text into edit mode.
   *
   * This method is only effective if the LabelEdit property of the ListView
   * control that contains the column is set to true. You can use this method at
   * run time to force the column's text to display in edit mode. For example, if
   * you are validating the column text edited by the user, and an column fails
   * validation, you could select the column in the ListView control and call
   * the BeginEdit method to force the user to change the text that failed
   * validation.
   *
   * @access  public
   * @return  boolean                 true if edit mode started
   */
  beginEdit : function() {
  },

  /**
   * Ensures that the column is visible within the control, scrolling the
   * contents of the control if necessary.
   * 
   * You can use this method to ensure that the column is visible within the
   * ListView control. This method can be used when performing validation on
   * the column. You can call the EnsureVisible method to ensure that the column
   * is displayed in the ListView control, if it failed validation, to allow
   * the user to perform changes on the column.
   *
   * This method is a shortcut to calling the ensureVisible() method of the
   * owning ListView control with the index of this column.
   *
   * @access  public
   * @return  void
   */
  ensureVisible : function() {
    var owner = this.owner();
    if (owner) {
      owner.ensureVisible(owner.columns().indexOf(this));
    }
  },

  /**
   * Removes the column from its associated list view control.
   * 
   * You can use this method to remove an column from its ListView control.
   *
   * The method is a short cut to the remove() method on the parent ListView
   * controls column collection.
   *
   * @access  public
   * @return  void
   */
  remove : function() {
    var owner = this.owner();
    if (owner) {
      owner.columns().remove(this);
    }
  },

  /**
   * Return the HTML markup required to render the object.
   *
   * This returns the markup required to render the object in Flow mode.
   *
   * @access  public
   * @return  string                  HTML markup for the object
   */
  _renderFlowColumn : function() {
    var i = this.index();
    var id = 'column#'+i;
    var name = 'Column '+i+' with a really long awkward name just to test the code';
    var icon = JUIC_DIR + '../../../media/images/icons/32x32/access.gif';
    var tooltip = 'Click me';

    var html = '\
      <!-- ListViewColumn -->\
      <div id="'+id+'" class="juic-listviewcolumn" juic="'+id+'"\
        title="'+tooltip+'">\
        <span class="juic-listviewcolumn-icon">\
          <span class="juic-listviewcolumn-icon-inner">\
            <img id="'+id+'_img" src="'+icon+'" />\
          </span>\
        </span>\
        <span class="juic-listviewcolumn-name">\
          <span class="juic-listviewcolumn-name-inner">\
            '+name+'\
          </span>\
        </span>\
      </div>\
      <!-- /ListViewColumn -->';

    return html;
  },

  /**
   * Return the HTML markup required to render the object.
   *
   * This returns the markup required to render the object in Grid mode.
   *
   * @access  public
   * @return  string                  HTML markup for the object
   */
  _renderGridColumn : function() {
  },

  /**
   * Return the HTML markup required to render the object.
   *
   * This returns a correctly formatted HTML markup string required to render
   * the ListViewColumn in the browser.
   *
   * @access  public
   * @return  string                  HTML markup for the object
   */
  toString : function() {
    if (this._listview.view == this._listview.VIEW.GRID) {
      return this._renderGridView();
    } else {
      return this._renderFlowView();
    }
  },

  /**
   * Get the correct icon url for the current column
   *
   * Checks the columns state etc. to return the correct image.
   *
   * @access  public
   * @return  string                  the columns icon url
   */ 
  _columnIcon : function(column, view) {
  },

  /**
   * Destructor
   *
   * @access  public
   * @return  void
   */
  _destructor : function() {
  }

});

/* vim: set expandtab tabstop=2 shiftwidth=2: */


