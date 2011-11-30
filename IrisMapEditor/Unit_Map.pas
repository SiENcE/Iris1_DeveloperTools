unit Unit_Map;

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

uses Classes, Graphics, SysUtils;

type
  TMapCell = packed record
      TileID: Word;
      Z: ShortInt;
  end;

  TMapBlock = packed record
      Header: Cardinal;
      Cells: array[0..7, 0..7] of TMapCell;
  end;

  TColor32 = packed record
      r, g, b, a: Byte;
  end;

  PCellArray = ^TCellArray;
  TCellArray = array[0..255] of TMapCell;

  PBlockArray = ^TBlockArray;
  TBlockArray = array[0..255] of TMapBlock;

  PCardinalArray = ^TCardinalArray;
  TCardinalArray = array[0..255] of Cardinal;

  TRadarCol = class (TObject)
      private
         FColors: array[0..16383] of TColor32;
         function GetColor (Index: Integer): TColor32;
      public
         property Colors[Index: Integer]: TColor32 read GetColor;
         constructor Create (AStream: TStream); overload;
         constructor Create (AFileName: String); overload;
  end;

  TMapType = (mtMap0, mtMap1, mtMap2, mtMap3, mtMap4);
  TMap = class (TObject)
      private
         FCells: PCellArray;
         FWidth, FHeight: Integer;
      public
         property Width: Integer read FWidth;
         property Height: Integer read FHeight;
         constructor Create (AWidth, AHeight: Integer); overload;
         constructor Create (AStream: TStream; AType: TMapType); overload;
         constructor Create (AFileName: String; AType: TMapType); overload;
         destructor Destroy; override;

         procedure Save (AStream: TStream); overload;
         procedure Save (AFileName: String); overload;
         procedure CreatePreview (ABitmap: TBitmap; AFactor: Integer; RadarCol: TRadarCol);
         function GetCell (X, Y: Integer): TMapCell;
         procedure IncreaseZ (X, Y: Integer; Z: Integer);
  end;

implementation

  function Color15to32 (AColor15: Word): TColor32;
  begin
     result.r := ((AColor15 shr 10) and $1F) * 8;  // red
     result.g := ((AColor15 shr 5) and $1F) * 8; // green
     result.b := (AColor15 and $1F) * 8;  // blue
     result.a := 255;
  end;

  constructor TRadarCol.Create (AStream: TStream);
  var
    Buffer: Array[0..16383] of Word;
    Index: Integer;
  begin
      Assert (Assigned (AStream));
      AStream.Read(Buffer, sizeof (Buffer));
      For Index := 0 to 16383 do
         FColors[Index] := color15to32 (Buffer[Index]);

  end;

  constructor TRadarCol.Create (AFileName: String);
  var
     Stream: TStream;
  begin
     Stream := TFileStream.Create(AFileName, fmOpenRead);
     try
        Create (Stream);
     finally
        Stream.Free;
     end;
  end;

  function TRadarCol.GetColor (Index: Integer): TColor32;
  begin
      if (Index < 0) or (Index >= 16384) then
          raise Exception.Create ('Invalid Tile');
      result := FColors[Index];
  end;

  constructor TMap.Create (AWidth, AHeight: Integer);
  begin
    Assert (AWidth > 0);
    Assert (AHeight > 0);
    FWidth := AWidth;
    FHeight := AHeight;
    GetMem (FCells, AWidth * AHeight * sizeof(TMapCell));
    FillChar (FCells^, AWidth * AHeight * sizeof(TMapCell), 0);
  end;

  constructor TMap.Create (AStream: TStream; AType: TMapType);
  var
    X, Y, BlockSizeX, BlockSizeY, Index: Integer;
    Buffer: PBlockArray;
  begin
      Assert (Assigned (AStream));
      case AType of
          mtMap0: begin
            BlockSizeX := 896; //896 for M.Legacy
            BlockSizeY := 512;
          end;
          mtMap1: begin
            BlockSizeX := 768; //old map0.mul
            BlockSizeY := 512;
          end;
          mtMap2: begin
            BlockSizeX := 288;
            BlockSizeY := 200;
          end;
          mtMap3: begin
            BlockSizeX := 320;
            BlockSizeY := 256;
          end;
          mtMap4: begin
            BlockSizeX := 181;
            BlockSizeY := 181;
          end;
          else
              raise Exception.Create ('Unknown map format');
      end;

      Create (BlockSizeX * 8, BlockSizeY * 8);

      GetMem (Buffer, sizeof (TMapBlock) * BlockSizeY);
      try

        For X := 0 to BlockSizeX - 1 do begin
            AStream.Read(Buffer^, sizeof (TMapBlock) * BlockSizeY);
            For Y := 0 to BlockSizeY - 1 do begin
                For Index := 0 to 7 do
                 Move (Buffer^[Y].Cells[Index, 0], FCells^[X * 8 + (Y * 8 + Index) * FWidth], sizeof (TMapCell) * 8);
          end;
      end;
      finally
          FreeMem (Buffer);
      end;
  end;

  constructor TMap.Create (AFileName: String; AType: TMapType);
  var
     Stream: TStream;
  begin
     Stream := TFileStream.Create(AFileName, fmOpenRead);
     try
        Create (Stream, AType);
     finally
        Stream.Free;
     end;

  end;

  destructor TMap.Destroy;
  begin
    FreeMem (FCells);
  end;

  procedure TMap.CreatePreview (ABitmap: TBitmap; AFactor: Integer; RadarCol: TRadarCol);
  var
    X, Y, dX, dY: Integer;
    Offset: Integer;
    Color: TColor32;
    r, g, b: Integer;
  begin
      Assert (Assigned (ABitmap));
      Assert (Assigned (RadarCol));

      Assert (AFactor > 0);
      ABitmap.Width := FWidth div AFactor;
      ABitmap.Height := FHeight div AFactor;
      ABitmap.PixelFormat := pf32bit;
      For Y := 0 to FHeight div AFactor - 1 do begin
          For X := 0 to FWidth div AFactor - 1 do begin

              r := 0; g := 0; b := 0; // Antialiasing
              For dY := 0 to AFactor - 1 do begin
                  Offset := X * AFactor + (Y * AFactor + dY) * FWidth;
                  For dX := 0 to AFactor - 1 do begin
                      Color := RadarCol.Colors[FCells[Offset + dX].TileID];
                      inc (R, Color.R); inc (G, Color.G); inc (B, Color.B);
                  end;
              end;

              R := R div (AFactor * AFactor);
              G := G div (AFactor * AFactor);
              B := B div (AFactor * AFactor);

              ABitmap.Canvas.Pixels[X, Y] := R + G shl 8 + B shl 16;
          end;
      end;
  end;

  function TMap.GetCell (X, Y: Integer): TMapCell;
  begin
      FillChar (Result, sizeof (Result), 0);
      if (X >= 0) and (X < FWidth) and (Y >= 0) and (Y < FHeight) then begin
          Result := FCells[X + Y * FWidth];
      end;
  end;

  procedure TMap.IncreaseZ (X, Y: Integer; Z: Integer);
  var
    Value: Integer;
  begin
      if (X >= 0) and (X < FWidth) and (Y >= 0) and (Y < FHeight) then begin
          Value := FCells[X + Y * FWidth].Z;
          inc (Value, Z);
          if Value > 127 then Value := 127;
          if Value < -127 then Value := -127;
          FCells[X + Y * FWidth].Z := Value;
      end;
  end;


  procedure TMap.Save (AStream: TStream);
  var
    X, Y, BlockSizeX, BlockSizeY, Index: Integer;
    Buffer: PBlockArray;
  begin
      Assert (Assigned (AStream));

      BlockSizeX := FWidth div 8;
      BlockSizeY := FHeight div 8;

      GetMem (Buffer, sizeof (TMapBlock) * BlockSizeY);
      try

        For X := 0 to BlockSizeX - 1 do begin
            For Y := 0 to BlockSizeY - 1 do begin
               For Index := 0 to 7 do
                 Move (FCells^[X * 8 + (Y * 8 + Index) * FWidth], Buffer^[Y].Cells[Index, 0], sizeof (TMapCell) * 8);
          end;
            AStream.Write(Buffer^, sizeof (TMapBlock) * BlockSizeY);
      end;
      finally
          FreeMem (Buffer);
      end;
  end;

  procedure TMap.Save (AFileName: String);
  var
     Stream: TStream;
  begin
     Stream := TFileStream.Create(AFileName, fmCreate);
     try
        Save (Stream);
     finally
        Stream.Free;
     end;
  end;

end.
