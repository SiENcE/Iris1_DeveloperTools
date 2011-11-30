unit Unit_Compatibility;
//
// Created by: Alexander Oster - tensor@ultima-iris.de
//
(*****
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****)

// This old and ugly unit is from the old editor only for compatibility reasons

interface

uses Inifiles, SysUtils, classes, contnrs,  Unit_OpenGL, Unit_OpenGLGeometry;

const
 FILEID_MODEL: array[0..3] of Char = ('C','M','#','2');
 FILEID_MODEL_OLD: array[0..3] of Char = ('C','M','#','1');
 FILEID_STATIC: array[0..3] of Char = ('C','S','#','2');
 FILEID_STATIC_OLD: array[0..3] of Char = ('C','S','#','1');
 FILEID_TEXTURES: array[0..3] of Char = ('C','T','#','2');
 FILEID_TEXTURES_OLD: array[0..3] of Char = ('C','T','#','1');

 QUADFLAG_BACKFACE = 1;
 QUADFLAG_TRIANGLE = 2;
 QUADFLAG_NEARONLY = 4;

 MODELFLAG_ISGROUND    = 1;
 MODELFLAG_TILEABLE    = 2;

type
  T2DVertex = array[0..1] of Single;

PBufferArray = ^TBufferArray;
TBufferArray = array[0..1023] of TObject;

TBuffer = class (TObject)
 private
  FMaxIndex: Integer;
  FBufferArray: PBufferArray;
 public
  property MaxIndex: Integer read FMaxIndex;
  constructor Create; overload;
  constructor Create(MaxIndex: Integer); overload;
  destructor Destroy; override;
  procedure Clear;
  procedure Add(Index: Integer; Entry: TObject);
  procedure Delete(Index: Integer);
  function Get(Index: Integer): TObject;
end;

  TModelInfos = record
    Height: Integer;
    Flags: Integer;
    Weight: Byte;
    Quality: Byte;
    Quantity: Byte;
    AnimID: Word;
    Hue: Byte;
    name: array[0..19] of char;
    CullingFlags: array[0..5] of Boolean;
  end;

  TQuadVertex = record
   vertex: TVertex;
   u, v: Single;
   normal: TVertex;
  end;

  PQuad = ^TQuad;
  TQuad = packed record
   Texture: Integer;
   Marked: Integer;
   Vertieces: array[0..3] of TQuadVertex;
   TextureCoords: T2DVertex;
   TextureSize: T2DVertex;
   Flags: Integer;
  end;

  TQuad2 = record
   Texture: Integer;
   Marked: Boolean;
   Vertieces: array[0..3] of TQuadVertex;
   TextureCoords: T2DVertex;
   TextureSize: T2DVertex;
   Flags: Integer;
  end;

  PQuadArray = ^TQuadArray;
  TQuadArray = array[0..2047] of TQuad;

  PGLTextureArray = ^TGLTextureArray;
  TGLTextureArray = array[0..2047] of TGLuInt;

  TModelFileHeader = packed record
   ID: array[0..3] of Char;
   QuadCount: Integer;
   Infos: TModelInfos;
   Flags: Integer;
   Reserved: array[0..35] of Byte;
  end;

  TModelFileHeader_Load = packed record
   ID: array[0..3] of Char;
   QuadCount: Integer;
   Infos: TModelInfos;
   Flags: Integer;
   Reserved: array[0..35] of Byte;
  end;

  TModelFileHeader_Load1 = packed record
   ID: array[0..3] of Char;
   QuadCount: Integer;
   Infos: TModelInfos;
   ModelIsTile: Boolean;
   Reserved: array[0..38] of Byte;
  end;

  TStaticFileHeader = packed record
   ID: array[0..3] of Char;
   ModelCount: Integer;
   TextureStart: Integer;
   Reserved: array[0..8] of Integer;
  end;

  TTextureFileHeader = packed record
   ID: array[0..3] of Char;
   Count: Integer;
   Reserved: array[0..39] of Byte;
  end;

  TTextureFileEntry = packed record
   name: array[0..59] of Char;
   ColorKey : Integer;
   AuthorID: Word;
   TexID: Word;
  end;

  TStaticFileIndexEntry = record
   Index: Integer;
   Position: Integer;
  end;

  TStaticTexture = class
   name: String;
   GLTex: TGLuInt;
   AuthorID: Word;
   TexID: Word;
   ColorKey : Integer;
   procedure ReLoad;
   constructor Create;
  end;

  PStaticFileIndex = ^TStaticFileIndex;
  TStaticFileIndex=array[0..2047] of TStaticFileIndexEntry;

  TGLTextureList = class(TObject)
   private
//    FTextureArray: PGLTextureArray;
    FTextures: TObjectList;
    function GetCount: Integer;
   public
    property Count: Integer read GetCount;
    constructor Create;
    destructor Destroy; override;
    function Add(Name: String; Tex: TGLuInt; AuthorID: Word): Integer;
    function Get(Index: Integer): TStaticTexture;
    procedure Delete(Index: Integer);
    function GetGLTex(Index: Integer): TGLuInt;
    function GetName(Index: Integer): String;
    procedure LoadFromBin(From: TStream);
  end;


  TStaticModel = class(TObject)
   private
    FQuadCount: Integer;
    FQuads: PQuadArray;
    FTextures: TGLTextureList;
   public
    FInfos: TModelInfos;
    FFlags: Integer;
    FCustomTexture: TGLuInt;
    property QuadCount: Integer read FQuadCount write FQuadCount;
    property Textures: TGLTextureList read FTextures write FTextures;
    constructor Create;
    destructor Destroy; override;
    procedure LoadFromBin(From: TStream);
    function GetQuad(Index: Integer): PQuad;
    procedure DeleteQuad(Index: Integer);
    procedure AddQuad(Quad: TQuad);
    procedure SetQuadCount(ACount: Integer);
    procedure SetQuad(Index: Integer; Quad: TQuad);
  end;

  TStaticEngine = class(TObject)
   FStaticBuffer: TBuffer;
   FTextureList: TGLTextureList;
   constructor Create;
   destructor Destroy; override;
   procedure Load(Filename: String; TextureFilename: String);
   function Get(Index: Integer): TStaticModel;
   procedure Delete(Index: Integer);
   procedure Add(Index: Integer; Model: TStaticModel);
  end;

  var
      StaticEngine: TStaticEngine;


implementation


constructor TBuffer.Create;
begin
 Create(1024);
end;

constructor TBuffer.Create(MaxIndex: Integer);
begin
 FMaxIndex := MaxIndex;
 GetMem(FBufferArray, sizeof(TObject)*FMaxIndex);
 fillchar(FBufferArray^, sizeof(TObject)*FMaxIndex, 0);
end;

destructor TBuffer.Destroy;
begin
 Clear;
 FreeMem(FBufferArray);
end;

procedure TBuffer.Clear;
var
 Index: Integer;
begin
 for Index:=0 to FMaxIndex-1 do begin
  FBufferArray[Index].Free;
  FBufferArray[Index]:=nil;
 end;
end;

function TBuffer.Get(Index: Integer): TObject;
begin
 if (Index < 0) or (Index>=FMaxIndex) then
    result:= nil
  else
    result:=FBufferArray[Index];
end;

procedure TBuffer.Add(Index: Integer; Entry: TObject);
begin
 if (Index >= 0) and (Index<FMaxIndex) then begin
   if Assigned(FBufferArray[Index]) then
     FBufferArray[Index].Free;
   FBufferArray[Index]:=Entry;
 end;
end;

procedure TBuffer.Delete(Index: Integer);
begin
 Add(Index, nil);
end;

    function CheckID(ID1, ID2: array of Char): Boolean;
    var
     Index: Integer;
    begin
     result:=True;
     For Index:=0 to 3 do
      result:=result and (ID1[Index] = ID2[Index]);
    end;

    constructor TStaticTexture.Create;
    begin
      ColorKey:=-1;
    end;

    procedure TStaticTexture.Reload;
    begin
      glDeleteTextures(1, @GLTex);
    end;

    constructor TGLTextureList.Create;
    begin
     FTextures:=TObjectList.Create;
//     FTextureCount:=Count;
//     GetMem(FTextureArray, sizeof(TGLuInt) * Count);
    end;

    destructor TGLTextureList.Destroy;
    begin
      FTextures.Free;
//     FreeMem(FTextureArray);
    end;

    function TGLTextureList.Add(Name: String; Tex: TGLuInt; AuthorID: Word): Integer;
    Var
     Index: Integer;
     NewID: Word;
     Texture: TStaticTexture;
    begin
     Name:=AnsiLowerCase(Name);
     NewID:=0;
     for Index:=0 to FTextures.Count-1 do begin
      Texture:=Get(Index);
      if Texture.Name = Name then begin
       result:=Index;
       Texture.GLTex:=Tex;
       exit;
      end;
      if (Texture.AuthorID = AuthorID) and (Texture.TexID > NewID) then
          NewID:=Texture.TexID;
     end;
     inc(NewID);
     Texture:=TStaticTexture.Create;
     Texture.Name:=Name;
     Texture.GLTex:=Tex;
     Texture.AuthorID:=AuthorID;
     Texture.TexID:=NewID;
     Texture.ColorKey:=-1;

     result:=FTextures.Count;
     FTextures.Add(Texture);
//     if (Index>=0) and (Index < FTextureCount) then
//      FTextureArray[Index]:=Tex;
    end;

    procedure TGLTextureList.Delete(Index: Integer);
    begin
      FTextures.Delete(Index);
    end;

    function TGLTextureList.Get(Index: Integer): TStaticTexture;
    begin
     result:=nil;
     if (Index>=0) and (Index < FTextures.Count) then
      result:=TStaticTexture(FTextures[Index]);
    end;

    function TGLTextureList.GetName(Index: Integer): String;
    begin
     result:='';
     if (Index>=0) and (Index < FTextures.Count) then
      result:=Get(Index).name;
    end;

    function TGLTextureList.GetGLTex(Index: Integer): TGLuInt;
    begin
     result:=0;
     if (Index>=0) and (Index < FTextures.Count) then
      result:=Get(Index).GLTex;
    end;

    function TGLTextureList.GetCount: Integer;
    begin
     result:=FTextures.Count;
    end;

    procedure TGLTextureList.LoadFromBin(From: TStream);
    var
     Header: TTextureFileHeader;
     Entry: TTextureFileEntry;
     FTexCount: Integer;
     Index, j: Integer;
     Texture: TStaticTexture;
    begin
     FTextures.Clear;

     From.Read(Header, sizeof(Header));

     if CheckID(Header.ID, FILEID_Textures) or CheckID(Header.ID, FILEID_Textures_OLD) then begin
      FTexCount:=Header.Count;
     end else
      FTexCount:=0;


     if (FTexCount > 0) then begin
       For Index:= 0 to FTexCount-1 do begin
        Texture:=TStaticTexture.Create;
        From.Read(Entry, sizeof(Entry));
        Texture.name:='';
        for j:=0 to 59 do if Entry.name[j]<>#0 then
         Texture.name:=Texture.name+Entry.name[j];
        Texture.ColorKey := -1;
        if (CheckID(Header.ID, FILEID_Textures)) then
          Texture.ColorKey := Entry.ColorKey;
//        Texture.Load;

        Texture.AuthorID:=Entry.AuthorID;
        Texture.TexID:=Entry.TexID;
        FTextures.Add(Texture);
       end;
     end
    end;


    constructor TStaticModel.Create;
    begin
     FQuadCount:=0;
     FQuads:=nil;
     FTextures:=nil;
     FFlags:=0;
    end;

    destructor TStaticModel.Destroy;
    begin
     if Assigned(FQuads) then
      FreeMem(FQuads);
    end;


    procedure TStaticModel.LoadFromBin(From: TStream);
    var
     Header: TModelFileHeader_Load;
     Header1: TModelFileHeader_Load1 absolute Header;
     Index: Integer;
     Quad: TQuad2;
    begin
     if Assigned(FQuads) then
       FreeMem(FQuads);
     From.Read(Header, sizeof(Header));
     if CheckID(Header.ID, FILEID_Model) or CheckID(Header.ID, FILEID_Model_OLD) then begin
      FQuadCount:=Header.QuadCount;
      FInfos:=Header.Infos;
      if CheckID(Header.ID, FILEID_Model_old) then begin
        if(Header1.ModelIsTile) then
          FFlags:=MODELFLAG_ISGROUND else FFlags:=MODELFLAG_TILEABLE;
      end else begin
         FFlags:=Header.Flags;
//         if (FFlags <> MODELFLAG_ISGROUND) then
//          FFlags := MODELFLAG_TILEABLE;
      end;

     end else
      FQuadCount:=0;

     if (FQuadCount > 0) then begin
       GetMem(FQuads, FQuadCount * sizeof(TQuad));
       For Index:= 0 to FQuadCount-1 do begin
        fillchar(FQuads^[Index], sizeof(TQuad), 0);
        From.Read(Quad,sizeof(TQuad2));
//        if (Quad.Flags > 4) then
//         showmessage(Inttostr(Quad.Flags));
        Quad.Flags:=Quad.Flags and (QUADFLAG_BACKFACE or QUADFLAG_NEARONLY or QUADFLAG_TRIANGLE) ;
        move(Quad, FQuads^[Index], sizeof(TQuad2));
       end;
     end else
      FQuads := nil;
    end;



   function TStaticModel.GetQuad(Index: Integer): PQuad;
   begin
    result:=nil;
    if (Index>=0) and (Index < FQuadCount) then
     result:=@FQuads[Index];
   end;

   procedure TStaticModel.DeleteQuad(Index: Integer);
   var
    NewQuads: PQuadArray;
    I: Integer;
    Nr: Integer;
   begin
    if (Index>=0) and (Index < FQuadCount) then begin
     GetMem(NewQuads, (FQuadCount-1) * sizeof(TQuad));
     Nr:=0;
     For I:=0 to FQuadCount-1 do if (I<>Index) then begin
       NewQuads[Nr]:=FQuads[I];
       inc(Nr);
     end;
     FreeMem(FQuads);
     FQuads:=NewQuads;
     dec(FQuadCount);
    end;
   end;

   procedure TStaticModel.AddQuad(Quad: TQuad);
   var
    NewQuads: PQuadArray;
   begin
     GetMem(NewQuads, (FQuadCount+1) * sizeof(TQuad));
     Move(FQuads^, NewQuads^, FQuadCount * sizeof(TQuad));
     FreeMem(FQuads);
     FQuads:=NewQuads;
     FQuads[FQuadCount]:=Quad;
     inc(FQuadCount);
   end;

   procedure TStaticModel.SetQuadCount(ACount: Integer);
   var
    NewQuads: PQuadArray;
    I: Integer;
   begin
     I:=ACount;
     if FQuadCount < I then I:=FQuadCount;
     FQuadCount:=ACount;
     GetMem(NewQuads, FQuadCount * sizeof(TQuad));
     fillchar(NewQuads^,FQuadCount * sizeof(TQuad), 0);
     Move(FQuads^, NewQuads^, I * sizeof(TQuad));
     FreeMem(FQuads);
     FQuads:=NewQuads;
   end;


   procedure TStaticModel.SetQuad(Index: Integer; Quad: TQuad);
   begin
    if (Index>=0) and (Index < FQuadCount) then
     FQuads[Index]:=Quad;
   end;



   constructor TStaticEngine.Create;
   begin
    FStaticBuffer:=TBuffer.Create(49152);
    FTextureList:=nil;
   end;

   destructor TStaticEngine.Destroy;
   begin
    FTextureList.Free;
    FStaticBuffer.Free;
   end;

   procedure TStaticEngine.Load(Filename: String; TextureFilename: String);
   var
    Stream: TStream;
    Index: Integer;
    Model: TStaticModel;
    Header: TStaticFileHeader;
    FileIndex: PStaticFileIndex;
   begin
//    showmessage(Inttostr(sizeof(TQuad)));
    Stream:=TFileStream.Create(Filename, fmOpenRead);
    Stream.Read(Header, sizeof(Header));

    if CheckID(Header.ID, FILEID_STATIC) then begin
      FTextureList:=TGLTextureList.Create;
      Stream.Position:=Header.TextureStart;
      FTextureList.LoadFromBin(Stream);
      Stream.Position:=sizeof(Header);
    end;



    GetMem(FileIndex, sizeof(TStaticFileIndexEntry) * Header.ModelCount);
    Stream.Read(FileIndex^, sizeof(TStaticFileIndexEntry) * Header.ModelCount);
    For Index:=0 to Header.ModelCount-1 do begin
     Stream.Position:=FileIndex^[Index].Position;
     Model:=TStaticModel.Create;
     Model.FTextures:=FTextureList;
     Model.LoadFromBin(Stream);
     FStaticBuffer.Add(FileIndex^[Index].Index, Model);
    end;

    FreeMem(FileIndex);

    Stream.Free;
   end;


   function TStaticEngine.Get(Index: Integer): TStaticModel;
   begin
    result:=TStaticModel(FStaticBuffer.Get(Index));
   end;

   procedure TStaticEngine.Delete(Index: Integer);
   begin
    FStaticBuffer.Delete(Index);
   end;

   procedure TStaticEngine.Add(Index: Integer; Model: TStaticModel);
   begin
    if Assigned(Model) then
      Model.FTextures:=FTextureList;
    FStaticBuffer.Add(Index, Model);
   end;


end.
