# Operation
- Set Operations: can be implemented with Disjoint-Set/Hash Table/Other methods([Exchanging](http://blog.sina.com.cn/s/blog_616e189f0100mrdn.html), [Compression](http://ramsey16.net/%E4%BA%A4%E9%9B%86%E5%92%8C%E5%B9%B6%E9%9B%86/))
- Add support for dragging multiple files in
# UI
## Docks
- Selection View (TreeView) (Source View included)
    - Display the Affiliation between sources and segments / among segments
    - Root items are Source1, Source2, .., Cross-Segments(whose points from multiple sources)
    - If all of points from a segment belong to a single source, then the segment belong to the source
    -------------
    - Multiple selection should be supported
    - Sources/Segments should support solo/disable-view, solo/disable-selection (like in Adobe Illustrator)
    - Cross-segment should support being divided by sources.
- Selection Info
    - Statistic information (Points count/mean/centroid/etc.)
    - Source Information (The source file info)
    - Labeling
- Label List (TableView)
    - Label name editing
    - Label ID/Color
    - Label Statistics
- ~~Primitives~~ (Not supported currently)
    - Primitive shapes that divide the point cloud. (Plane/Box/Sphere)
    - Boolean Operations are supported.
    - Because boolean operation of shapes can be transferred into boolean operation of points, thus this is not taken into consideration now
## Toolbar Standard
- ToolTips show the help of Toolbutton
- Statusbar show the status of Toolbutton/Action
- Color toolbar support coloring by Segment/Label/Other origin fields(XYZ, RGB, intensity, etc).
- Primitive division toolbar