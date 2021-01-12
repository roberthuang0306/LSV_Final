## 一些我寫 cell 跟 grid 的時候預設外面的 class 要做的事情

+ 提供以 name 搜 MasterCell* 的 hashMap
+ 提供以 name 搜 Cell* 的 hashMap
+ 以一個二維陣列存 GridColumn*
+ 以一個一維陣列存每個 layer 的 supply
+ 在處理 nonDefaultSupply 的時候用改變 grid demand 的方式處理（call **GridColumn.addDemand()** or **GridColumn.subtractDemand()**）