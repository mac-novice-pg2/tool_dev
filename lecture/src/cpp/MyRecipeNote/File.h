#pragma once
class File{
    File *fp_;
public:
    File();
    virtual ~File();

    virtual void Open( const char *filename ) = 0;
    virtual void Close() = 0;
    virtual void Read() = 0;
    virtual void Write() = 0;
};

class RecipeFile : public File{

};

class TotalCostFile : public File{

};