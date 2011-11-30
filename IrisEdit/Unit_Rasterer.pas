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

unit Unit_Rasterer;

interface

uses Unit_OpenGLGeometry, Classes, Contnrs;

type

  TRaster3D = class (TObject)
  private
     FRaster: PIntegerArray;
     FSizeX, FSizeY, FSizeZ: Integer;
     FMinX, FMinY, FMinZ: Integer;
     FFactorX, FFactorY: Integer;
     FMemory: Integer;
     procedure SetRaster (X, Y, Z, Value: Integer);
     function GetRaster (X, Y, Z: Integer): Integer;

  public
     property Raster[X, Y, Z: Integer]: Integer read GetRaster write SetRaster; default;
     property SizeX: Integer read FSizeX;
     property SizeY: Integer read FSizeY;
     property SizeZ: Integer read FSizeZ;
     property MinX: Integer read FMinX;
     property MinY: Integer read FMinY;
     property MinZ: Integer read FMinZ;
     constructor Create (AMinX, AMinY, AMinZ, AMaxX, AMaxY, AMaxZ: Single);
     destructor Destroy; override;

     procedure RenderTriangle (V1, V2, V3: TVector3f; Len1, Len2, Len3: Single);

     procedure SaveToStream (AStream: TStream);
     procedure FloodFill;
     function CreateFuzzyRaster (AFactor: Integer; AShadowIntensity: Integer): TRaster3D;
  end;

  procedure CopyRaster (DstStream, SrcStream: TStream);


implementation


const
  RasterMax = 29;
  FloodFillStackCount = 16384;

type
  PFloodFillStackEntry = ^TFloodFillStackEntry;
  TFloodFillStackEntry = packed record
      X, Y, Z: Integer;
  end;

  PFloodFillStackEntryArray = ^TFloodFillStackEntryArray;
  TFloodFillStackEntryArray = array[0..FloodFillStackCount - 1] of TFloodFillStackEntry;

  PIntegerArray = ^TIntegerArray;
  TIntegerArray = array[0..16383] of Integer;

  TFloodFillStack = class (TObject)
  private
     FStackList: TStack;
     FCurrentStackPos: Integer;
     FCurrentStack: PFloodFillStackEntryArray;
  public
     constructor Create;
     destructor Destroy; override;
     procedure Push (X, Y, Z: Integer);
     function Pop: PFloodFillStackEntry;
  end;

  procedure CopyRaster (DstStream, SrcStream: TStream);
  var
    FSizeX, FSizeY, FSizeZ: Integer;
  begin
      SrcStream.Read(FSizeX, sizeof (FSizeX));
      SrcStream.Read(FSizeY, sizeof (FSizeY));
      SrcStream.Read(FSizeZ, sizeof (FSizeZ));
      Assert (FSizeX >= 0);
      Assert (FSizeY >= 0);
      Assert (FSizeZ >= 0);
      DstStream.Write (FSizeX, sizeof (FSizeX));
      DstStream.Write (FSizeY, sizeof (FSizeY));
      DstStream.Write (FSizeZ, sizeof (FSizeZ));
      DstStream.CopyFrom(SrcStream, 3 * 4); // MinX, MinY, MinZ
      DstStream.CopyFrom(SrcStream, FSizeX * FSizeY * FSizeZ); // Raster Data
  end;

constructor TFloodFillStack.Create;
begin
   FStackList := TStack.Create;
   New (FCurrentStack);
   FCurrentStackPos := 0;
end;

destructor TFloodFillStack.Destroy;
var
  Index: Integer;
begin
   For Index := 0 to FStackList.Count - 1 do
      Dispose(FStackList.Pop);
   Dispose (FCurrentStack);
   FStackList.Free;
end;

procedure TFloodFillStack.Push (X, Y, Z: Integer);
var
  AStackEntry: PFloodFillStackEntry;
begin
   if FCurrentStackPos >= FloodFillStackCount then begin
      FStackList.Push(FCurrentStack);
      New (FCurrentStack);
      FCurrentStackPos := 0;
   end;

   AStackEntry := @FCurrentStack^[FCurrentStackPos];
   AStackEntry^.X := X;
   AStackEntry^.Y := Y;
   AStackEntry^.Z := Z;
   inc (FCurrentStackPos);
end;

function TFloodFillStack.Pop: PFloodFillStackEntry;
begin
   if FCurrentStackPos = 0 then begin
      if FStackList.Count = 0 then begin
          result := nil;
          exit;
      end;
      Dispose (FCurrentStack);
      FCurrentStack := FStackList.Pop;
      FCurrentStackPos := FloodFillStackCount - 1;
   end;

   dec (FCurrentStackPos);
   result := @FCurrentStack^[FCurrentStackPos];
end;

constructor TRaster3D.Create (AMinX, AMinY, AMinZ, AMaxX, AMaxY, AMaxZ: Single);
begin
    Assert (AMinX < AMaxX);
    Assert (AMinY < AMaxY);
    Assert (AMinZ < AMaxZ);
    FMinX := trunc (AMinX); // Umrandung lassen
    FMinY := trunc (AMinY);
    FMinZ := trunc (AMinZ);
    FSizeX := trunc (AMaxX + 1) - FMinX; // Umrandung lassen + aufrunden
    FSizeY := trunc (AMaxY + 1) - FMinY;
    FSizeZ := trunc (AMaxZ + 1) - FMinZ;
    FFactorX := FSizeY * FSizeZ;
    FFactorY := FSizeZ;
    FMemory := FSizeX * FSizeY * FSizeZ;
    GetMem (FRaster, FMemory * sizeof (Integer));
    FillChar (FRaster^, FMemory * sizeof (Integer), 0);
end;

destructor TRaster3D.Destroy;
begin
    FreeMem (FRaster);
end;

procedure TRaster3D.SetRaster (X, Y, Z, Value: Integer);
var
   FPos: Integer;
begin
   Assert ((X >= 0) and (X < FSizeX));
   Assert ((Y >= 0) and (Y < FSizeY));
   Assert ((Z >= 0) and (Z < FSizeZ));
   FPos := X * FFactorX + Y * FFactorY
    + Z;
   Assert((FPos >= 0) and (FPos < FMemory));
   FRaster^[FPos] := Value;
end;

function TRaster3D.GetRaster (X, Y, Z: Integer): Integer;
var
   FPos: Integer;
begin
   FPos := X * FFactorX + Y * FFactorY + Z;
   Assert((FPos >= 0) and (FPos < FMemory));
   result := FRaster^[FPos];
end;

procedure TRaster3D.RenderTriangle (V1, V2, V3: TVector3f; Len1, Len2, Len3: Single);
var
  MidV1, MidV2, MidV3: TVector3f;
begin
  SetRaster(trunc(V1[0]) - FMinX, trunc(V1[1]) - FMinY, trunc(V1[2]) - FMinZ, 1);
  SetRaster(trunc(V2[0]) - FMinX, trunc(V2[1]) - FMinY, trunc(V2[2]) - FMinZ, 1);
  SetRaster(trunc(V3[0]) - FMinX, trunc(V3[1]) - FMinY, trunc(V3[2]) - FMinZ, 1);

  if (Len1 > 1) or (Len2 > 1) or (Len3 > 1) then begin
      Len1 := Len1 / 2;
      Len2 := Len2 / 2;
      Len3 := Len3 / 2;
      MidV1 := VectorScale(VectorAdd(V1, V2), 0.5);
      MidV2 := VectorScale(VectorAdd(V2, V3), 0.5);
      MidV3 := VectorScale(VectorAdd(V3, V1), 0.5);
      RenderTriangle (MidV1, MidV2, MidV3, Len1, Len2, Len3);
      RenderTriangle (V1, MidV1, MidV3, Len1, Len2, Len3);
      RenderTriangle (V2, MidV2, MidV1, Len1, Len2, Len3);
      RenderTriangle (V3, MidV3, MidV2, Len1, Len2, Len3);
  end;
end;


procedure TRaster3D.SaveToStream (AStream: TStream);
var
  Data: PByteArray;
  Index: Integer;
begin
  Assert (Assigned (AStream));
  AStream.Write(FSizeX, sizeof (FSizeX));  // Please change in "CopyRaster", too
  AStream.Write(FSizeY, sizeof (FSizeY));
  AStream.Write(FSizeZ, sizeof (FSizeZ));
  AStream.Write(FMinX, sizeof (FMinX));
  AStream.Write(FMinY, sizeof (FMinY));
  AStream.Write(FMinZ, sizeof (FMinZ));
  GetMem (Data, FMemory);
  try
      For Index := 0 to FMemory - 1 do begin
          Assert ((FRaster^[Index] >= 0) and (FRaster^[Index] <= 255));
          Data^[Index] := FRaster^[Index];
      end;

      AStream.Write(Data^, FMemory);
  finally
     FreeMem(Data);
  end;
end;

procedure TRaster3D.FloodFill;
var
  X, Y, Z: Integer;
  Stack: TFloodFillStack;
  Entry: PFloodFillStackEntry;
begin
        Stack := TFloodFillStack.Create;
        try
           Stack.Push(0, 0, 0); // In einer Ecke beginnen
           SetRaster(0, 0, 0, 2); // Initialisieren
           Entry := Stack.Pop;
           While Assigned (Entry) do begin
                X := Entry^.X; Y := Entry^.Y; Z := Entry^.Z;
                if GetRaster (X, Y, Z) = 2 then begin // Wenn Füllpunkt
                    if X < (FSizeX - 1) then
                        if GetRaster (X + 1, Y, Z) = 0 then begin
                            SetRaster (X + 1, Y, Z, 2);
                            Stack.Push(X + 1, Y, Z);
                        end;
                    if Y < (FSizeY - 1) then
                        if GetRaster (X, Y + 1, Z) = 0 then begin
                            SetRaster (X, Y + 1, Z, 2);
                            Stack.Push(X, Y + 1, Z);
                        end;
                    if Z < (FSizeZ - 1) then
                        if GetRaster (X, Y, Z + 1) = 0 then begin
                            SetRaster (X, Y, Z + 1, 2);
                            Stack.Push(X, Y, Z + 1);
                        end;
                    if X > 0 then
                        if GetRaster (X - 1, Y, Z) = 0 then begin
                            SetRaster (X - 1, Y, Z, 2);
                            Stack.Push(X - 1, Y, Z);
                        end;
                    if Y > 0 then
                        if GetRaster (X, Y - 1, Z) = 0 then begin
                            SetRaster (X, Y - 1, Z, 2);
                            Stack.Push(X, Y - 1, Z);
                        end;
                    if Z > 0 then
                        if GetRaster (X, Y, Z - 1) = 0 then begin
                            SetRaster (X, Y, Z - 1, 2);
                            Stack.Push(X, Y, Z - 1);
                        end;
                end;
                Entry := Stack.Pop;
           end;
        finally
           Stack.Free;
        end;

        For X := 0 to FSizeX - 1 do
          For Y := 0 to FSizeY - 1 do
            For Z := 0 to FSizeZ - 1 do
                if GetRaster (X, Y, Z) = 2 then
                    SetRaster (X, Y, Z, 0)
                else
                    SetRaster (X, Y, Z, 1);
end;

// Erstellt ein "analoges" Raster aus einem feinen Raster
function TRaster3D.CreateFuzzyRaster (AFactor: Integer; AShadowIntensity: Integer): TRaster3D;
var
  AMinX, AMinY, AMinZ: Integer;
  AMaxX, AMaxY, AMaxZ: Integer;
  RelX, RelY, RelZ: Integer;
  X, Y, Z: Integer;
  dX, dY, dZ, V, ax, ay, az: Integer;
  AShadowIntensityFactor: Single;
begin
  AShadowIntensityFactor := 1;
  if AShadowIntensity > 0 then
    AShadowIntensityFactor := AShadowIntensity * 5;
  if AShadowIntensity < 0 then
    AShadowIntensityFactor := 1 / abs (AShadowIntensity * 5);

  assert (AFactor > 1);
  AMinX := trunc (FMinX / AFactor);
  AMinY := trunc (FMinY / AFactor);
  AMinZ := trunc (FMinZ / AFactor);
  AMaxX := trunc (FSizeX / AFactor) + AMinX;
  AMaxY := trunc (FSizeY / AFactor) + AMinY;
  AMaxZ := trunc (FSizeZ / AFactor) + AMinZ;
  RelX := FMinX - AMinX * AFactor;
  RelY := FMinY - AMinY * AFactor;
  RelZ := FMinZ - AMinZ * AFactor;
  result := TRaster3D.Create(AMinX, AMinY, AMinZ, AMaxX, AMaxY, AMaxZ);
  For Z := 0 to result.SizeZ - 1 do
    For Y := 0 to result.SizeY - 1 do
      For X := 0 to result.SizeX - 1 do begin
          V := 0;
          For dZ := 0 to AFactor - 1 do
            For dY := 0 to AFactor - 1 do
              For dX := 0 to AFactor - 1 do begin
                 ax := X * AFactor + dX + RelX;
                 ay := Y * AFactor + dY + RelY;
                 az := Z * AFactor + dZ + RelZ;
                 if (ax >= 0) and (ay >= 0) and (az >= 0) and (ax < FSizeX) and (ay < FSizeY) and (az < FSizeZ) then
                  if GetRaster (ax, ay, az) <> 0 then
                      inc (V);
              end;
          V := round(((V * 255) div (AFactor * AFactor * AFactor)) * AShadowIntensityFactor);
          if V > 255 then V := 255;
          result.SetRaster(X, Y, Z, V);
      end;
end;

end.
