## クラス図

```plantuml
@startuml

' メニュー操作系
class Menu{
    +Show()
    +Clear()
    +InputKey()
}

class SearchMenu{

}

class TotalCostMenu{

}

Menu <|-- SearchMenu
Menu <|-- TotalCostMenu

' ファイルアクセス系
class File{
    fp : FILE*
    +Open() : filename
    +Close()
    +Read()
    +Write()
}

class RecipeFile{

}

class TotalCostFile{

}

File <|-- RecipeFile
File <|-- TotalCostFile

' 検索情報系
class SearchInfo{
    ItemNo : int
    料理名 : string
    調理時間 : int
    費用 : int
    kcal : int
    ジャンル : eGenre
    形式 : eType
    難易度 : eDifficulty
    シーズン : eSeason
    +ToString()
    +GetRecipe() : int ItemNo
    +GetIngredients() : int ItemNo
}
note right : 材料検索は将来対応

class Recipe{
    -検索情報リスト : SearchInfo[]
    -file : File
    +Search() : string dish
}
Recipe *-- SearchInfo
Recipe *-- File

'家計簿
class TotalCost{
    +Add()
    +GetCost()
}

' メインモジュール
class Main{
    -menu : Menu
    -recipe : Recipe
    -total_cost : TotalCost
}
Main *-- Menu
Main *-- Recipe
Main *-- TotalCost

@enduml
```

## コンポーネント

```plantuml
@startuml

家計簿 "家計簿"{
    package "package"{
        []

    }
}

cloud "Cloud" {
  package "Package" {
    [register]
    frame "frame" {
      [backup]
    }
  }
}

node "Node" {
  database "Database" {
    [store]
  }
  folder "Folder" {
    [File]
  }
}

[register] .. [store] : HTTP
[backup] .. [File] : FTP

@enduml
```
