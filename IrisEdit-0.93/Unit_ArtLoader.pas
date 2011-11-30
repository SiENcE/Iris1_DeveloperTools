unit Unit_ArtLoader;
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

uses Windows, Classes, Graphics, SysUtils;

type
   TArtLoader = class (TObject)
   private
       FArtStream, FArtIdxStream: TStream;
   public
       constructor Create (AArtFile, AArtIdx: String);
       destructor Destroy; override;
       procedure ShowTile (TileID: Integer; ACenter: TPoint; ACanvas: TCanvas);
       procedure SaveTileToBitmap (ABitmap: TBitmap; TileID: Integer);
   end;

var
   pArtLoader: TArtLoader;

implementation

type
  TIndexRecord = packed record
     Start, Len, Extra: Cardinal;
  end;

function Color15to32 (AColor15: Word): TColor;
begin
   result := ((AColor15 shr 10) and $1F) * 8 +  // red
             ((AColor15 shr 5) and $1F) * 8 * 256 + // green
             (AColor15 and $1F) * 8 * 65536;  // blue
end;

constructor TArtLoader.Create (AArtFile, AArtIdx: String);
begin
     FArtStream := TFileStream.Create (AArtFile, fmOpenRead or fmShareDenyWrite);
     FArtIdxStream := TFileStream.Create (AArtIdx, fmOpenRead or fmShareDenyWrite);
end;

destructor TArtLoader.Destroy;
begin
     FArtStream.Free;
     FArtStream := nil;
     FArtIdxStream.Free;
     FArtIdxStream := nil;
end;

procedure TArtLoader.ShowTile (TileID: Integer; ACenter: TPoint; ACanvas: TCanvas);
var
  IndexRecord: TIndexRecord;
  Flag: Integer;
  Width, Height: Word;
  LookupTable: array[0..1023] of Word;
  Buffer: Array[0..2047] of Word;
  X, Y: Integer;
  dY, I: Integer;
  DStart: Integer;
  xOfs, Run: Word;

begin
 Assert (Assigned (ACanvas));

 FArtIdxStream.Position := TileID * 12;
 FArtIdxStream.Read(IndexRecord, 12);

 if IndexRecord.Len > $FFFF then
    raise Exception.Create ('invalid art #' + IntToStr (TileID));

 FArtStream.Position:=IndexRecord.Start;
 FArtStream.Read(Flag, sizeof(Flag));

 I := 0;

 if (Flag>0) and (Flag<=$FFFF) then begin

   FArtStream.Read(Width, 2);
   FArtStream.Read(Height, 2);
   if (Width > 1023) or (Height > 1023) then
      raise Exception.Create ('invalid art size');

   FArtStream.Read(Lookuptable, Height * 2);

   DStart := FArtStream.Position;
   X := 0; Y := 0;

   FArtStream.Position := Lookuptable[Y] * 2 + DStart;
   while (Y < Height) do begin
       FArtStream.Read(XOfs, 2);
       FArtStream.Read(Run, 2);

       if (XOfs + Run >= 2048) then
          raise Exception.Create ('art load error');

       if (XOfs + Run <> 0) then begin
           inc (X, XOfs);
           FArtStream.Read(Buffer, Run * 2);
           for I:= 0 to Run - 1 do begin
               // Convert
               ACanvas.Pixels[X + I - Width div 2 + ACenter.X,
                              Y - Height div 2 + ACenter.Y] := Color15to32 (Buffer[I]);
           end;
           inc (X, Run);
       end else begin
           X := 0;
           inc (Y);
           FArtStream.Position := Lookuptable[Y] * 2 + DStart;
       end;
    end;

 end else begin
    if IndexRecord.Len <> 4096 then
       raise Exception.Create ('invalid art entry');

    FArtStream.Position := IndexRecord.Start;
    FArtStream.Read(Buffer, IndexRecord.Len);
    for Y := 0 to 43 do begin
       dY := Y;
       if (dY > 21) then dY := 43 - Y;
       for X := 0 to (dY + 1) * 2 - 1 do begin
            ACanvas.Pixels[X + (21 - dY) + ACenter.X - 22,
                           Y + ACenter.Y - 22] := Color15to32 (Buffer [I]);
            inc (I);
       end;
    end;
 end;

end;

procedure TArtLoader.SaveTileToBitmap (ABitmap: TBitmap; TileID: Integer);
var
  IndexRecord: TIndexRecord;
  Flag: Integer;
  Width, Height: Word;

begin
 Assert (Assigned (ABitmap));

 FArtIdxStream.Position := TileID * 12;
 FArtIdxStream.Read(IndexRecord, 12);

 if IndexRecord.Len > $FFFF then
    raise Exception.Create ('invalid art #' + IntToStr (TileID));

 FArtStream.Position:=IndexRecord.Start;
 FArtStream.Read(Flag, sizeof(Flag));

 if (Flag>0) and (Flag<=$FFFF) then begin

   FArtStream.Read(Width, 2);
   FArtStream.Read(Height, 2);
   if (Width > 1023) or (Height > 1023) then
      raise Exception.Create ('invalid art size');

   ABitmap.Width := Width;
   ABitmap.Height := Height;
   ShowTile(TileID, Point(Width div 2, Height div 2), ABitmap.Canvas);

 end else begin
   ABitmap.Width := 44;
   ABitmap.Height := 44;
   ShowTile(TileID, Point(Width div 2, Height div 2), ABitmap.Canvas);
 end;
end;

initialization
   pArtLoader := nil;

finalization
   pArtLoader.Free;
   pArtLoader := nil;

end.
