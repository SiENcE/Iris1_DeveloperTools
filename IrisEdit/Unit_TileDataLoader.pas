{==============================================================================

  Iris Model Editor - Copyright by Alexander Oster, tensor@ultima-iris.de

 The contents of this file are used with permission, subject to
 the Mozilla Public License Version 1.1 (the "License"); you may
 not use this file except in compliance with the License. You may
 obtain a copy of the License at
 http://www.mozilla.org/MPL/MPL-1.1.html

 Software distributed under the License is distributed on an
 "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 implied. See the License for the specific language governing
 rights and limitations under the License.

==============================================================================}

unit Unit_TileDataLoader;

interface

uses Classes, Contnrs, SysUtils;

type
  TTileDataFileEntry = packed record
      Flags: Cardinal;
      Weight: Byte;
      Quality: Byte;
      Unknown1: SmallInt;
      Unknown2: Byte;
      Quantity: Byte;
      Animation: SmallInt;
      Unknown3: Byte;
      Hue: Byte;
      Unknown4: Byte;
      Unknown5: Byte;
      Height: Byte;
      Name: array[0..19] of Char;
  end;

  TTileDataEntryBuffer = packed record
      Header: Integer;
      Entries: array[0..31] of TTileDataFileEntry;
  end;

  TTileDataEntry = class (TObject)
     private
        FFlags: Cardinal;
        FWeight: Byte;
        FQuality: Byte;
        FAnimation: SmallInt;
        FHue: Byte;
        FQuantity: Byte;
        FHeight: Byte;
        FName: String;
     public
        property Flags: Cardinal read FFlags;
        property Weight: Byte read FWeight;
        property Quality: Byte read FQuality;
        property Animation: SmallInt read FAnimation;
        property Hue: Byte read FHue;
        property Height: Byte read FHeight;
        property Name: String read FName;
        property Quantity: Byte read FQuantity;
        constructor Create (AFileEntry: TTileDataFileEntry);
  end;

  TTileDataLoader = class  (TObject)
      private
         FEntries: TObjectList;
         function GetCount: Integer;
         function Get (Index: Integer): TTileDataEntry;
      public
         property Count: Integer read GetCount;
         property Items[Index: Integer]: TTileDataEntry read Get; default;
         constructor Create (FileName: String);
         destructor Destroy; override;
  end;

  var
     pTileDataLoader: TTileDataLoader;

implementation

constructor TTileDataEntry.Create (AFileEntry: TTileDataFileEntry);
var
  AArray: array[0..21] of Char;
begin
   FillChar (AArray, 21, 0);
   Move (AFileEntry.Name, AArray, 20);
   FFlags := AFileEntry.Flags;
   FWeight := AFileEntry.Weight;
   FQuality := AFileEntry.Quality;
   FAnimation := AFileEntry.Animation;
   FHue := AFileEntry.Hue;
   FHeight := AFileEntry.Height;
   FQuantity := AFileEntry.Quantity;
   SetString(FName, AArray, StrLen (AArray));
end;

constructor TTileDataLoader.Create (FileName: String);
var
  Stream: TStream;
  Index, EntryIndex: Integer;
  Count: Integer;
  Buffer: TTileDataEntryBuffer;
begin
    FEntries := TObjectList.Create;
    Stream := TFileStream.Create (FileName, fmOpenRead or fmShareDenyWrite);
    try
        Stream.Position := 512 * (4 + 32 * 26);
        if (Stream.Size - Stream.Position) <> 512 * sizeof (TTileDataEntryBuffer) then
             raise Exception.Create ('Invalid TileData File');
        Count := (Stream.Size - Stream.Position) div sizeof (TTileDataEntryBuffer);
        FEntries.Capacity := Count * 32;
        For Index := 0 to Count - 1 do begin
            Stream.Read (Buffer, sizeof (Buffer));
            For EntryIndex := 0 to 31 do
               FEntries.Add (TTileDataEntry.Create (Buffer.Entries[EntryIndex]));
        end;
    finally
        Stream.Free;
    end;
end;

destructor TTileDataLoader.Destroy;
begin
  FEntries.Free;
  FEntries := nil;
end;

function TTileDataLoader.GetCount: Integer;
begin
   result := FEntries.Count;
end;

function TTileDataLoader.Get (Index: Integer): TTileDataEntry;
begin
   result := FEntries[Index] as TTileDataEntry;
end;


initialization
  pTileDataLoader := nil;

finalization
  pTileDataLoader.Free;
  pTileDataLoader := nil;

end.
