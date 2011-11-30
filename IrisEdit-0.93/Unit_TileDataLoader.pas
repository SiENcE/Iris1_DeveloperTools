unit Unit_TileDataLoader;
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
