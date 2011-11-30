unit Unit_TextureManager;
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

interface

uses Classes, Decal, md5, JPEG, Graphics, SysUtils, Contnrs, Unit_Compatibility,
     Dialogs, Unit_OpenGL, Unit_ArtLoader, Unit_IntegerList, Controls;

type
   PCardinalArray = ^TCardinalArray;
   TCardinalArray = array[0..255] of Cardinal;

   TProgressCallback = procedure (Percentage: Integer; Caption: String = '') of Object;

   TFileSign = array[0..3] of Char;

   TTexture = class (TObject)
       private
          FGlobalID: Cardinal;
          function GetGLTexture: Cardinal; virtual; abstract;
       public
          property GlobalID: Cardinal read FGlobalID;
          constructor Create;
   end;


   TJPEGTexture = class (TTexture)
       private
          FName: String;
          FMD5Sum: String;
          FStream: TStream;
          FWidth, FHeight: Integer;
          FHasColorKey: Boolean;
          FColorKey: TColor;
          FIndex: Integer;
          function GetGLTexture: Cardinal; override;
       public
          property Name: String read FName write FName;
          property MD5Sum: String read FMD5Sum;
          property Stream: TStream read FStream;
          property Width: Integer read FWidth;
          property Height: Integer read FHeight;
          property HasColorKey: Boolean read FHasColorKey write FHasColorKey;
          property ColorKey: TColor read FColorKey write FColorKey;
          property Index: Integer read FIndex write FIndex;
          constructor Create (AName: String; AStream: TStream; ASize: Cardinal);
          constructor CreateFromStream (AStream: TStream);
          destructor Destroy; override;
          function CreateBitmap: TBitmap;
          procedure Save (AStream: TStream);
   end;

   TArtTexture = class (TTexture)
       private
         FID: Cardinal;
         function GetGLTexture: Cardinal; override;
       public
         constructor Create (AID: Cardinal);
         destructor Destroy; override;
   end;

   TGLInstanceTextureList = class (TObject) // Handles the textures for an OpenGL instance
       private
         FMap: DMap;
         FInUseCounter: DMap;
       public
         property InUseCounter: DMap read FInUseCounter;
         constructor Create;
         destructor Destroy; override;
         function GetGLTexture (ATexture: TTexture): Cardinal;
         procedure Reset; // Reset use counter
   end;

   TTextureManager = class (TObject)
       private
         FMap: DMap;
         FArtTextures: DMap;
         FTextures: TObjectList;
         FGroundMap: DMap;
         function GetTextureCount: Integer;
         function GetTextureFromIndex (Index: Integer): TJPEGTexture;
       public
         property Count: Integer read GetTextureCount;
         property Textures [Index: Integer]: TJPEGTexture read GetTextureFromIndex; default;
         constructor Create;
         destructor Destroy; override;
         function AddTexture (AName: String; AStream: TStream; ASize: Cardinal): TJPEGTexture; overload;
         function AddTexture (AName: String; AFileName: String): TJPEGTexture; overload;
         function GetTexture (AMD5Sum: String): TJPEGTexture;

         function AddArtTexture (ID: Cardinal): TArtTexture;

         // Returns a Map [ OldTextureID -- NewTexture ]
         function ImportTextures (TextureList: TGLTextureList; ATextureDir: String; AProgressCallback: TProgressCallback): DMap;
         procedure Save (AStream: TStream; APatchXML: TStrings = nil);
         procedure Load (AStream: TStream);

         procedure AddGroundMap (GroundID: Integer; AMD5Sum: String);
         function GetGroundMap (GroundID: Integer): TJPEGTexture;
         function GetLinkedGroundIDs (AMD5Sum: String): TIntegerList;
   end;

   const
      CURRENT_VERSION = 2;

   var
      pTextureManager: TTextureManager;

implementation

const
   TextureFileSign: TFileSign = ('U', 'I', '3', 'F');
   TEXTUREFLAG_HASCOLORKEY = 1;
var
   GlobalIDCounter: Cardinal = 1;

type
   TTextureFileHeader = packed record
       Sign: TFileSign;
       Length: Cardinal;
       Version: Cardinal;
       TextureTableStart: Cardinal;
       TextureTableCount: Cardinal;
       GroundTableStart: Cardinal;
       GroundTableCount: Cardinal;
       Reserved: array[0..8] of Cardinal;
   end;

   TTextureTableEntry = packed record
       Start: Integer;
       Length: Integer;
   end;

   TGroundTextureTableEntry = packed record
       FID: Integer;
       FMD5Sum: array[0..31] of Char;
   end;

   TTextureFileEntry = packed record
       FName: array[0..31] of Char;
       FMD5Sum: array[0..31] of Char;
       FWidth, FHeight: Cardinal;
       Length: Integer;
       ColorKey: Cardinal;
       FFlags: Integer;
       Reserved: array[0..2] of Cardinal;
   end;
   PTextureTableEntryArray = ^TTextureTableEntryArray;
   TTextureTableEntryArray = array[0..16383] of TTextureTableEntry;

   function CreateTextureFromBitmap (ABitmap: TBitmap): Cardinal;
   var
      W, H, X, Y, P: Integer;
      Data: PCardinalArray;
      ScanLine: PCardinalArray;
      Value1, Value2: Cardinal;
   begin
        Assert(Assigned (ABitmap));
        Assert(ABitmap.PixelFormat = pf32bit);
        W := ((ABitmap.Width + 3) div 4) * 4;  // Round to 4
        H := ((ABitmap.Height + 3) div 4) * 4;
        GetMem (Data, W * H * Sizeof (Cardinal));
        try
           FillChar (Data^, W * H * Sizeof (Cardinal), 0);

           For Y := 0 to ABitmap.Height - 1 do begin
               ScanLine := ABitmap.ScanLine[Y];
               P := Y * W;
               For X := 0 to ABitmap.Width - 1 do begin
                   Value1 := ScanLine^[X];
                   Value2 := (Value1 shr 16) or (Value1 and $00FF00) or ((Value1 and $0000FF) shl 16);
                   Data^ [P + X] := Value2 or $FF000000;
               end;
           end;

           glGenTextures( 1, @result);
           glBindTexture( GL_TEXTURE_2D, result );

           glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);   { Texture blends with object background }
           glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); { only first two can be used  }
           glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); { all of the above can be used }
           gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, W, H, GL_RGBA, GL_UNSIGNED_BYTE, Data );
        finally
           FreeMem (Data);
        end;
   end;

   function CreateTextureFromArtBitmap (ABitmap: TBitmap): Cardinal;
   var
      W, H, X, Y, P: Integer;
      Data: PCardinalArray;
      ScanLine: PCardinalArray;
      Value1, Value2: Cardinal;
   begin
        Assert(Assigned (ABitmap));
        Assert(ABitmap.PixelFormat = pf32bit);
        W := ((ABitmap.Width + 3) div 4) * 4;  // Round to 4
        H := ((ABitmap.Height + 3) div 4) * 4;
        GetMem (Data, W * H * Sizeof (Cardinal));
        try
           FillChar (Data^, W * H * Sizeof (Cardinal), 0);

           For Y := 0 to ABitmap.Height - 1 do begin
               ScanLine := ABitmap.ScanLine[ABitmap.Height - 1 - Y];
               P := Y * W;
               For X := 0 to ABitmap.Width - 1 do begin
                   Value1 := ScanLine^[X];
                   Value2 := (Value1 shr 16) or (Value1 and $00FF00) or ((Value1 and $0000FF) shl 16);
                   Data^ [P + X] := Value2 or $FF000000;
               end;
           end;

           glGenTextures( 1, @result);
           glBindTexture( GL_TEXTURE_2D, result );

           glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);   { Texture blends with object background }
           glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); { only first two can be used  }
           glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); { all of the above can be used }
           gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, W, H, GL_RGBA, GL_UNSIGNED_BYTE, Data );
        finally
           FreeMem (Data);
        end;
   end;

   constructor TTexture.Create;
   begin
      inherited Create;
      FGlobalID := GlobalIDCounter;
      inc (GlobalIDCounter);
   end;

   constructor TJPEGTexture.Create (AName: String; AStream: TStream; ASize: Cardinal);
   var
     AContext: MD5Context;
     AInput: Pointer;
     ADigest: MD5Digest;
     Bitmap : TBitmap;
   begin
        inherited Create;
        FName := AName;

        Assert(Assigned (AStream));
        FStream := TMemoryStream.Create;
        FStream.CopyFrom (AStream, ASize);
        FStream.Position := 0;

        AInput := (FStream as TMemoryStream).Memory;

        MD5Init (AContext);
        MD5Update (AContext, PChar(AInput), FStream.Size);
        MD5Final (AContext, ADigest);
        FMD5Sum := MD5Print (ADigest);


        Bitmap := CreateBitmap;  // Check, if JPEG can be Loaded
        try
            FWidth := Bitmap.Width;
            FHeight := Bitmap.Height;
        finally
            Bitmap.Free;
        end;
   end;

   constructor TJPEGTexture.CreateFromStream (AStream: TStream);
   var
     FileEntry: TTextureFileEntry;
     Index: Integer;
   begin
      inherited Create;
      Assert(Assigned(AStream));
      AStream.Read (FileEntry, sizeof (FileEntry));
      For Index := 0 to 31 do
          if FileEntry.FName[Index] <> #0 then
             FName := FName + FileEntry.FName[Index]
          else
             break;
      For Index := 0 to 31 do
          FMD5Sum := FMD5Sum + FileEntry.FMD5Sum[Index];

      FWidth := FileEntry.FWidth;
      FHeight := FileEntry.FHeight;

      FHasColorKey := FileEntry.FFlags and TEXTUREFLAG_HASCOLORKEY <> 0;
      FColorKey := FileEntry.ColorKey;

      FStream := TMemoryStream.Create;
      FStream.CopyFrom (AStream, FileEntry.Length);
   end;

   destructor TJPEGTexture.Destroy;
   begin
        FStream.Free;
        FStream := nil;
   end;

   function TJPEGTexture.CreateBitmap: TBitmap;
   var
     JPEGImage: TJPEGImage;
   begin
        JPEGImage := TJPEGImage.Create;
        try
            FStream.Position := 0;
            JPEGImage.LoadFromStream (FStream);
            result := TBitmap.Create;
            result.PixelFormat := pf32bit;
            result.Width := JPEGImage.Width;
            result.Height := JPEGImage.Height;
            result.Canvas.Draw (0, 0, JPEGImage);
        finally
            JPEGImage.Free;
        end;
   end;


   function TJPEGTexture.GetGLTexture: Cardinal;
   var
     Bitmap: TBitmap;
   begin
         Bitmap := CreateBitmap;
         try
            result := CreateTextureFromBitmap (Bitmap);
         finally
            Bitmap.Free;
         end;
   end;

   procedure TJPEGTexture.Save (AStream: TStream);
   var
     FileEntry: TTextureFileEntry;
     Len, Index: Integer;
   begin
     Assert(Assigned(AStream));
     FillChar (FileEntry, sizeof (FileEntry), 0);

     Len := Length (FName);
     if Len > 32 then Len := 32;
     For Index := 0 to Len - 1 do
         FileEntry.FName[Index] := FName[Index + 1];

     Assert (Length (FMD5Sum) = 32);
     For Index := 0 to 31 do
         FileEntry.FMD5Sum[Index] := FMD5Sum[Index + 1];

     FileEntry.FWidth := Width;
     FileEntry.FHeight := Height;
     FileEntry.ColorKey := FColorKey;
     if FHasColorKey then
        FileEntry.FFlags := FileEntry.FFlags or TEXTUREFLAG_HASCOLORKEY;
     FileEntry.Length := FStream.Size;
     AStream.Write(FileEntry, sizeof(FileEntry));

     FStream.Position := 0;  // Copy Image Data
     AStream.CopyFrom (FStream, FStream.Size);

   end;

   function TArtTexture.GetGLTexture: Cardinal;
   var
     Bitmap: TBitmap;
   begin
         Bitmap := TBitmap.Create;
         Bitmap.PixelFormat := pf32Bit;
         try
           try
              Assert (Assigned(pArtLoader));
              pArtLoader.SaveTileToBitmap(Bitmap, FID + 16384);
              result := CreateTextureFromArtBitmap (Bitmap);
           finally
              Bitmap.Free;
           end;
         except
            result := 1;
         end;
   end;

   constructor TArtTexture.Create (AID: Cardinal);
   begin
        inherited Create;
        FID := AID;
   end;

   destructor TArtTexture.Destroy;
   begin
   end;

   constructor TGLInstanceTextureList.Create;
   begin
      FMap := DMap.Create;
      FInUseCounter := DMap.Create;
   end;

   destructor TGLInstanceTextureList.Destroy;
   var
      Iter: DIterator;
      Texture: Integer;
   begin
      Iter := FMap.start;
      while not atEnd (Iter) do begin
          Texture := getInteger (Iter);
          glDeleteTextures (1, @Texture);
          Advance (Iter);
      end;
      FMap.Free;
      FInUseCounter.Free;
   end;

   function TGLInstanceTextureList.GetGLTexture (ATexture: TTexture): Cardinal;
   var
      Iter: DIterator;
   begin
      Assert (Assigned (ATexture));
      Iter := FMap.locate([ ATexture.GlobalID ]);
      if atEnd (Iter) then begin
         result := ATexture.GetGLTexture;
         FMap.putPair([ ATexture.GlobalID, result ]);
      end else
         result := GetInteger (Iter);

      Iter := FInUseCounter.locate([ATexture]);
      if atEnd (Iter) then
         FInUseCounter.putPair([ATexture, 1])
      else begin
         SetToValue (Iter);
         FInUseCounter.putPair([ATexture, getInteger(Iter) + 1])
      end;
   end;

   procedure TGLInstanceTextureList.Reset;
   begin
      FInUseCounter.clear;
   end;

   constructor TTextureManager.Create;
   begin
      FMap := DMap.Create;
      FArtTextures := DMap.Create;
      FTextures := TObjectList.Create;
      FGroundMap := DMap.Create;
   end;

   destructor TTextureManager.Destroy;
   begin
      ObjFree(FArtTextures);
      FArtTextures.Free;
      FMap.Free;
      FMap := nil;
      FTextures.Free;
      FTextures := nil;
      FGroundMap.Free;
      FGroundMap := nil;
   end;

   function TTextureManager.AddTexture (AName: String; AStream: TStream; ASize: Cardinal): TJPEGTexture;
   var
      Texture: TJPEGTexture;
      Iter: DIterator;
   begin
        Assert (Assigned (AStream));
        Texture := TJPEGTexture.Create (AName, AStream, ASize);

        Iter := FMap.locate ([ Texture.MD5Sum ]);  // check, if texture is already in the map
        if atEnd (Iter) then begin
           FMap.putPair ([ Texture.MD5Sum, Texture ]);
           Texture.Index := FTextures.Count;
           FTextures.Add (Texture);
           result := Texture;
        end else begin
           Texture.Free;
           result := getObject (Iter) as TJPEGTexture;
        end;

   end;

   function TTextureManager.AddArtTexture (ID: Cardinal): TArtTexture;
   var
      Iter: DIterator;
   begin
        Iter := FArtTextures.locate ([ ID ]);  // check, if texture is already in the map
        if atEnd (Iter) then begin
           result := TArtTexture.Create(ID);
           FArtTextures.putPair ([ ID, result ]);
        end else begin
           result := getObject(Iter) as TArtTexture;
        end;

   end;

   function TTextureManager.AddTexture (AName: String; AFileName: String): TJPEGTexture;
   var
      AStream: TStream;
   begin
       AStream := TFileStream.Create (AFileName, fmOpenRead);
       try
          result := AddTexture (AName, AStream, AStream.Size);
       finally
          AStream.Free;
       end;
   end;

   function TTextureManager.GetTextureCount: Integer;
   begin
        result := FTextures.Count;
   end;

   function TTextureManager.GetTextureFromIndex (Index: Integer): TJPEGTexture;
   begin
        result := FTextures[Index] as TJPEGTexture;
   end;

   function TTextureManager.GetTexture (AMD5Sum: String): TJPEGTexture;
   var
     Iter: DIterator;
   begin
      Iter := FMap.locate ([AMD5Sum]);
      if not atEnd (Iter) then
         result := getObject (Iter) as TJPEGTexture
      else
         result := nil;
   end;

   function TTextureManager.ImportTextures (TextureList: TGLTextureList; ATextureDir: String; AProgressCallback: TProgressCallback): DMap;
   var
       Index: Integer;
       AOldTexture: TStaticTexture;
       ATexture: TJPEGTexture;
       AName: String;
   begin
       Assert(Assigned(TextureList));
       result := DMap.Create;
       For Index := 0 to TextureList.Count - 1 do begin
           AOldTexture := TextureList.Get(Index);
           AName := AOldTexture.Name;
           try
              ATexture := AddTexture (AOldTexture.Name, ATextureDir + '\' + AOldTexture.Name);
              ATexture.HasColorKey := AOldTexture.ColorKey >= 0;
              if ATexture.HasColorKey then
                  ATexture.ColorKey := AOldTexture.ColorKey;
              result.putPair ([Index, ATexture]);
           except
               On E: Exception do begin
                  if MessageDlg ('Error while loading ' + AName + ': ' + E.Message, mtError, [ mbOK, mbCancel ], 0) = mrCancel then
                      exit;
               end;
           end;
           if Assigned (AProgressCallback) then
              AProgressCallback(round (Index * 100 / TextureList.Count));
       end;
   end;

   procedure TTextureManager.Save (AStream: TStream; APatchXML: TStrings);
   var
      Header: TTextureFileHeader;
      Position: Integer;
      Texture: TJPEGTexture;
      TextureTable: PTextureTableEntryArray;
      Index: Integer;
      GroundTableEntry: TGroundTextureTableEntry;
      Iter: DIterator;
      MD5Sum: String;
   begin
     Assert (Assigned (AStream));
     Position := AStream.Position;

     FillChar(Header, sizeof (Header), 0);
     Header.Sign := TextureFileSign;
     Header.Version := CURRENT_VERSION;
     Header.TextureTableCount := FTextures.Count;

     AStream.Write (Header, sizeof(Header));

     Header.TextureTableStart := AStream.Position - Position;

     GetMem(TextureTable, FTextures.Count * sizeof (TTextureTableEntry));
     try
        FillChar (TextureTable^, FTextures.Count * sizeof (TTextureTableEntry), 0);
        AStream.Write (TextureTable^, FTextures.Count * sizeof (TTextureTableEntry));

        For Index := 0 to FTextures.Count - 1 do begin
           Texture := Textures[Index];
           TextureTable^[Index].Start := AStream.Position - Position;
           Texture.Save (AStream);
           TextureTable^[Index].Length := AStream.Position - Position - TextureTable^[Index].Start;
            if Assigned (APatchXML) then
                APatchXML.Add(Format('    <texture id="%d" offset="%d" length="%d" md5sum="%s">', [ Index, TextureTable^[Index].Start + Position, TextureTable^[Index].Length, Texture.MD5Sum ]));
        end;

        AStream.Position := Integer(Header.TextureTableStart) + Position;
        AStream.Write (TextureTable^, FTextures.Count * sizeof (TTextureTableEntry));

        AStream.Position := AStream.Size;
        Header.GroundTableStart := AStream.Position - Position;
        Header.GroundTableCount := FGroundMap.size;
        Iter := FGroundMap.start;
        while not atEnd(Iter) do begin
            SetToKey(Iter);
            GroundTableEntry.FID := getInteger (Iter);
            SetToValue(Iter);
            MD5Sum := getString(Iter);
            Assert (Length (MD5Sum) = 32);
            For Index := 0 to 31 do
                 GroundTableEntry.FMD5Sum[Index] := MD5Sum[Index + 1];
            AStream.Write(GroundTableEntry, sizeof(GroundTableEntry));
            Advance (Iter);
        end;


        Header.Length := AStream.Position - Position;

        AStream.Position := Position;
        AStream.Write (Header, sizeof(Header));
        AStream.Position := AStream.Size;

     finally
        FreeMem (TextureTable);
     end;
end;

procedure TTextureManager.Load (AStream: TStream);
   var
      Header: TTextureFileHeader;
      Texture: TJPEGTexture;
      TextureTable: PTextureTableEntryArray;
      Index, I: Integer;
      GroundTableEntry: TGroundTextureTableEntry;
      MD5Sum: String;
      Position: Integer;
   begin
     Position := AStream.Position;

     Assert (Assigned (AStream));
     AStream.Read(Header, sizeof (Header));
     Assert (Header.Sign = TextureFileSign, 'invalid texture file');
     if Header.Version > CURRENT_VERSION then
        raise Exception.Create ('This file needs a newer version of this editor');

     GetMem(TextureTable, Header.TextureTableCount * sizeof (TTextureTableEntry));
     try
         AStream.Position := Integer(Header.TextureTableStart) + Position;
         AStream.Read (TextureTable^, Header.TextureTableCount * sizeof (TTextureTableEntry));
         For Index := 0 to Header.TextureTableCount - 1 do begin
             AStream.Position := TextureTable^[Index].Start + Position;
             Texture := TJPEGTexture.CreateFromStream (AStream);
             Texture.Index := Index;
             FMap.putPair ([Texture.MD5Sum, Texture]);
             FTextures.Add(Texture);
         end;
     finally
        FreeMem (TextureTable);
     end;

     AStream.Position := Integer(Header.GroundTableStart) + Position;
     For Index := 0 to Header.GroundTableCount - 1 do begin
         AStream.Read(GroundTableEntry, sizeof(GroundTableEntry));
         MD5Sum := '';
         For I := 0 to 31 do
            MD5Sum := MD5Sum + GroundTableEntry.FMD5Sum[I];
         AddGroundMap (GroundTableEntry.FID, MD5Sum);
     end;
end;

procedure TTextureManager.AddGroundMap (GroundID: Integer; AMD5Sum: String);
var
  Texture: TJPEGTexture;
begin
    if (GroundID < 0) or (GroundID >= 16384) then
        raise Exception.Create('Invalid Ground ID');

    FGroundMap.remove([GroundID]);
    if AMD5Sum <> '' then begin
        Texture := GetTexture (AMD5Sum);
        if Assigned(Texture) then
          FGroundMap.putPair([GroundID, AMD5Sum]);
    end;
end;

function TTextureManager.GetGroundMap (GroundID: Integer): TJPEGTexture;
var
  Iter: DIterator;
begin
  result := nil;
  Iter := FGroundMap.locate([GroundID]);
  if not atEnd (Iter) then
      result := GetTexture(getString(Iter));
end;

function TTextureManager.GetLinkedGroundIDs (AMD5Sum: String): TIntegerList;
var
  Iter: DIterator;
begin
  result := TIntegerList.Create;
  Iter := FGroundMap.start;
  while not atEnd (Iter) do begin
      if getString (Iter) = AMD5Sum then begin
        SetToKey (Iter);
        result.Add(getInteger(Iter));
        SetToValue (Iter);
      end;
      Advance(Iter);
  end;
end;

initialization
   pTextureManager := TTextureManager.Create;

finalization
   pTextureManager.Free;
   pTextureManager := nil;


end.
