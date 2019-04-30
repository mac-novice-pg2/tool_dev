#pragma once

#include <vector>

#include "SearchType.h"

struct SearchCondition{
    int item_no;
    char name[ 32 ];
    int time;
    int cost;
    int kcal;
    eFoodGenre genre;
    eSeason season;
    eDifficulty difficulty;
    eFoodStyle type;

    SearchCondition()
        : item_no( 0 ), name( "" ), time( -1 ), cost( -1 ), kcal( -1 ),
        genre( eFoodGenre::Invalid_Food ),
        season( eSeason::Invalid_Season ),
        difficulty( eDifficulty::Invalid_Difficulty ),
        type( eFoodStyle::Invalid_FoodStyle )
    {}

    bool IsMatch_name( const char *check );
    bool IsMatch_time( int check );
    bool IsMatch_cost( int check );
    bool IsMatch_kcal( int check );
    bool IsMatch_genre( eFoodGenre check );
    bool IsMatch_season( eSeason check );
    bool IsMatch_difficulty( eDifficulty check );
    bool IsMatch_type( eFoodStyle check );
};

struct SearchInfo{
    int item_no;
    char name[ 32 ];
    int time;
    int cost;
    int kcal;
    char kitchenware[ 32 ];

    eFoodGenre genre;
    eSeason season;
    eDifficulty difficulty;
    eFoodStyle type;

    char recipe[ 1024 ];
    char ingredients[ 256 ];

    SearchInfo()
        : item_no( 0 ), name( "" ), time( 0 ), cost( 0 ), kcal( 0 ),
        genre( eFoodGenre::Japanese_Food ),
        season( eSeason::AllSeason ),
        difficulty( eDifficulty::Easy_Difficulty ),
        type( eFoodStyle::AllSituation )
    {}

    const char *ToString();
};

using SearchInfoList = std::vector< SearchInfo >;

class Recipe{
    SearchInfoList search_info_;
    SearchInfoList search_result_;

    void set_numeric_field( FILE *fp, int *out );
    const char* read_string_param( FILE *fp );

    void create_search_list( const char *recipe_fname );
    SearchInfo read_entry( FILE *fp );
    Recipe();

public:
    Recipe( const char *recipe_fname );
    virtual ~Recipe();

    void ShowDishList();
    const SearchInfoList& Search( SearchCondition *cond );
};
