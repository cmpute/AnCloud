# UI
## Docks
- Selection View (TreeView) (Source View included)
    - Object : Display the Affiliation between sources and segments / among segments
    - Root items are Source1, Source2, .., Cross-Segment1, CroCross-Segment2, ...(whose points from multiple sources)
    - If all of points from a segment belong to a single source, then the segment belong to the source
    - Every selection should, if it has a child selection, contains a remainder selection which contains points that has not been selected. If the selection is covered by all children, then the remainder disappear.
    - Selection icons include: Source, unlabelled selection, labelled selection, unlabelled cross-selection, labelled cross-selection, remainder.
    -------------
    - Multiple selection should be supported
    - Sources/Segments should support solo/disable-view, solo/disable-selection (like in Adobe Illustrator, it could be left click is disable switch while right click is solo switch)
    - Cross-segment should support being divided into parts by source.
    - Supported set operations are: Merge(Union), Substract(Difference), Intersection, Toggle(Symmetric Difference), Complement
- Selection Info
    - Statistic information (Points count/mean/centroid/etc.)
    - Source Information (The source file info)
    - Labeling
    - Display customize
- Label List (TableView)
    - Label name editing
    - Label ID/Color
    - Label Statistics
- Spread Sheet
    - Point data
    - Considering support selection in spread sheet?
- ~~Primitives~~ (Not supported currently)
    - Primitive shapes that divide the point cloud. (Plane/Box/Sphere)
    - Boolean Operations are supported.
    - Because boolean operation of shapes can be transferred into boolean operation of points, thus this is not taken into consideration now
## Rendering
- Unselectable(Locked/Archived) selection should be transparent

# Implementation
## Algorithm
- The set operation could be boosted by using a bitvector<WSIZE> (`vector<bitset>`), which the WSIZE (word size) is preferred to be 32? (should check in [bitset source code](https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-3.4/bitset-source.html))(VTKBitArray doesn't support bit operations)
- 【What structure shoud be used to store labels in selection? Label ID or just directly Label string?】
## Classes that need implementation
- `acSelectionManager`
- `acSelectionModel` (for TreeView)
- `vtkSelectionOperation` (for high-performance set operations)
- `acDataRepresentation` / `vtkSMACPresentation` (may be needed if selection will be not native in ParaView, and add support for data-input color information)
## Classes that need reimplementation
- `vtkExtractSelection`:
    1. support bitset indicators
    1. when build an extraction, automatically add to selection manager.
- `pqObjectBuilder`:
    1. support customized representation
    1. hook with selection manager

# Standards
## Toolbar Standard
- ToolTips show the help of Toolbutton
- Statusbar show the status of Toolbutton/Action
- Color toolbar support coloring by Segment/Label/Other origin fields(XYZ, RGB, intensity, etc).
- Primitive division toolbar
## Shortkey Standard
- Frequently used keys should be assigned to single key, like selection buttons, selection modifier buttons, etc.

# Plugins
## Selection Plugin
- SelectionOperationFilter (MergeSelection/IntersectSelection/ToggleSelection)
    1. original source should be the same (which means the ExtractSelection filter should preserve the topology to keep the original source and offer a "vtkInsidedness" field). If not, the implement should use a unordered_set to implement which cost a lot and is C++11 only.
    1. the filter itself should have a preserve topology option as well.
- SelectedPointsRepresentation
    1. When processed on certain source, it will simply filter the vtkInsidedness and show selected part.
- Selection Toolbar (Considering)
    1. Add quick access to use selection operation filter
    1. Add button to merge original selections of selectionOperationFilter/ExtractSelectionFilter by one level. (Call it 'Merge Up')

# Others
- Add data field palette presets like MATLAB, add random (hash with given seed) palette by integers (not gradient platte).
- Add support for dragging multiple files in

# Candidate pull requests to ParaView
1. [Feature]add "intersection" selection modifier
1. [Issue]add toggle button to set `UseMultipleRepresentationSelection` property in `pqRenderView`.【should be easy to implement】
1. [Issue]if object is invisible, then updating the object will not trigger update in statics inspector. 
1. [Issue]`AddFieldArray` should be able to add a field by user assign, not just by reading file. (Is there another easy way to add a field?)
1. [Feature]Add boolean operation filter (there is a implementation in VTK, just port it to ParaView)
1. Add icons to sources, and add interactive constuction of basic sources (box, cylinder, etc), just like what Clip filter does.
1. [Feature]Add `GridSource`
1. [Feature]Offer option for select `VTK_POINT_SHELL`/`VTK_POINT_UNIFORM` in `Points` source
