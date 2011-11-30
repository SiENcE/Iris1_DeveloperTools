unit Unit_Matrix;
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

uses SysUtils, Unit_OpenGLGeometry;

type
  EMatrixError = class (Exception);
  EVectorError = class (Exception);

  TDeterminant = array[0..8] of Single;

  TMVector = class (TObject)
      private
        FFields: array[0..2] of Single;
        function GetField (Index: Integer): Single;
        function GetX: Single;
        function GetY: Single;
        function GetZ: Single;
        procedure SetField (Index: Integer; Value: Single);
        procedure SetX (Value: Single);
        procedure SetY (Value: Single);
        procedure SetZ (Value: Single);
      public
        property Fields[Index: Integer]: Single read GetField write SetField; default;
        property X: Single read GetX write SetX;
        property Y: Single read GetY write SetY;
        property Z: Single read GetZ write SetZ;
        constructor Create; overload;
        constructor Create (X, Y, Z: Single); overload;
        constructor Create (AGLVector: TVector3f); overload;
        constructor CreateFrom (Vector: TMVector);
        constructor CreateAddition (Vector1, Vector2: TMVector);
        constructor CreateSubtraction (Vector1, Vector2: TMVector);
        procedure Normalize;
        procedure Add (AVector: TMVector);
        procedure Subtract (AVector: TMVector);
        procedure Assign(AVector: TMVector);
  end;

  TMRay = class (TObject)
     private
       FOrigin, FDir: TMVector;
     public
       property Origin: TMVector read FOrigin;
       property Dir: TMVector read FDir;
       constructor Create;
       destructor Destroy; override;
       function InterSectWithPlane (vecP, vecNormal: TVector3f): TMVector;
       function InterSectWithTriangle (vecP1, vecP2, vecP3: TVector3f; Var lambda: Single): Boolean;
  end;

  TMMatrix = class (TObject)
     private
       FFields: array[0..3, 0..3] of Single;
       function GetField(X, Y: Integer): Single;
       procedure SetField(X, Y: Integer; Value: Single);
     public
       property Fields[X, Y: Integer]: Single read GetField write SetField; default;
       constructor Create;
       constructor CreateFrom(Matrix: TMMatrix);
       constructor CreateScaleMatrix (Vector: TMVector);
       constructor CreateTranslationMatrix (Vector: TMVector);
       constructor CreateRotationMatrix (AVector: TMVector; Angle: Single);
       constructor CreateInversedMatrix (AMatrix: TMMatrix);
       constructor CreateFromGLMatrix (AMatrix: TMatrix4f);
       destructor Destroy; override;
       procedure Identity;
       procedure Zero;
       procedure MultiplyRight (AMatrix: TMMatrix);
       procedure MultiplyLeft (AMatrix: TMMatrix);
       function Inverse: TMMatrix;
       function Apply (AVector: TMVector): TMVector; overload;
       function ApplyRotation (AVector: TMVector): TMVector; overload;
       function Apply (ARay: TMRay): TMRay; overload;
       procedure Apply (Var X, Y, Z: Single); overload;
       procedure ApplyRotation (Var X, Y, Z: Single); overload;
       procedure Assign(AMatrix: TMMatrix);
  end;

implementation

function CalcDeterminant (detD: TDeterminant): Single;
begin
	result :=  + (detD[0]*detD[4]*detD[8])
		   + (detD[1]*detD[5]*detD[6])
		   + (detD[2]*detD[3]*detD[7])
		   - (detD[6]*detD[4]*detD[2])
		   - (detD[7]*detD[5]*detD[0])
		   - (detD[8]*detD[3]*detD[1]);
end;

constructor TMVector.Create;
begin
end;

constructor TMVector.Create (X, Y, Z: Single);
begin
   FFields[0] := X;
   FFields[1] := Y;
   FFields[2] := Z;
end;

constructor TMVector.Create (AGLVector: TVector3f);
var
  Index: Integer;
begin
  For Index := 0 to 2 do
     FFields[Index] := AGLVector[Index];
end;

constructor TMVector.CreateFrom (Vector: TMVector);
begin
   Assign(Vector);
end;

constructor TMVector.CreateAddition (Vector1, Vector2: TMVector);
begin
     CreateFrom(Vector1);
     Add(Vector2);
end;

constructor TMVector.CreateSubtraction (Vector1, Vector2: TMVector);
begin
     CreateFrom(Vector1);
     Subtract(Vector2);
end;

function TMVector.GetField (Index: Integer): Single;
begin
   if (Index < 0) or (Index > 2) then
      raise EVectorError.Create('GetField out of range');
   result := FFields[Index];
end;

function TMVector.GetX: Single;
begin
    result := FFields[0];
end;

function TMVector.GetY: Single;
begin
    result := FFields[1];
end;

function TMVector.GetZ: Single;
begin
    result := FFields[2];
end;

procedure TMVector.SetField (Index: Integer; Value: Single);
begin
   if (Index < 0) or (Index > 2) then
      raise EVectorError.Create('GetField out of range');
   FFields[Index] := Value;
end;

procedure TMVector.SetX (Value: Single);
begin
   FFields[0] := Value;
end;

procedure TMVector.SetY (Value: Single);
begin
   FFields[1] := Value;
end;

procedure TMVector.SetZ (Value: Single);
begin
   FFields[2] := Value;
end;

procedure TMVector.Normalize;
var
  Len: Single;
  Index: Integer;
begin
  Len := sqrt (FFields[0] * FFields[0] + FFields[1] * FFields[1] + FFields[2] * FFields[2]);
  if Len = 0 then
     raise EVectorError.Create ('tried to normalize Zero vector');
  For Index := 0 to 2 do
     FFields[Index] := FFields[Index] / Len;
end;

procedure TMVector.Add (AVector: TMVector);
var
  Index: Integer;
begin
   Assert(Assigned(AVector));
   For Index := 0 to 2 do
      FFields[Index] := FFields[Index] + AVector[Index];
end;

procedure TMVector.Subtract (AVector: TMVector);
var
  Index: Integer;
begin
   Assert(Assigned(AVector));
   For Index := 0 to 2 do
      FFields[Index] := FFields[Index] - AVector[Index];
end;

procedure TMVector.Assign(AVector: TMVector);
var
  Index: Integer;
begin
  Assert(Assigned(AVector));
  For Index := 0 to 2 do
      FFields[Index] := AVector.Fields[Index];
end;


constructor TMRay.Create;
begin
   FOrigin := TMVector.Create;
   FDir := TMVector.Create;
end;

destructor TMRay.Destroy;
begin
   FOrigin.Free;
   FOrigin := nil;
   FDir.Free;
   FDir := nil;
end;

function TMRay.InterSectWithPlane (vecP, vecNormal: TVector3f): TMVector;
var
  divisor: Single;
  lambda: Single;
  Index: Integer;
begin
   result := nil;
   divisor := ((vecNormal[0] * FDir[0]) + (vecNormal[1] * FDir[1]) + (vecNormal[2] * FDir[2]));
   if divisor = 0 then
      exit;

   lambda := - (vecNormal[0] * (FOrigin[0] - vecP[0]) +
                vecNormal[1] * (FOrigin[1] - vecP[1]) +
                vecNormal[2] * (FOrigin[2] - vecP[2])) / divisor;

   result := TMVector.Create;
   For Index := 0 to 2 do
      result[Index] := FOrigin[Index] + lambda * FDir[Index];
end;

function TMRay.InterSectWithTriangle (vecP1, vecP2, vecP3: TVector3f; Var lambda: Single): Boolean;
var
  vecU, vecV, vecX: TVector3f;
  Index: Integer;
  detD: TDeterminant;
  DMain, Dlambda, Dalpha, Dbeta, alpha, beta: Single;
  vecNormal: TVector3f;
begin
  result := false;

  for Index := 0 to 2 do begin
      vecU[Index] := vecP2[Index] - vecP1[Index];
      vecV[Index] := vecP3[Index] - vecP1[Index];
  end;

  vecNormal := VectorCrossProduct (vecU, vecV);

  detD[0] := vecU[0];   detD[1] := vecV[0];   detD[2] := - FDir[0];
  detD[3] := vecU[1];   detD[4] := vecV[1];   detD[5] := - FDir[1];
  detD[6] := vecU[2];   detD[7] := vecV[2];   detD[8] := - FDir[2];
  DMain := CalcDeterminant (detD);
  if DMain = 0 then
     exit;

  detD[2] := FOrigin[0] - vecP1[0];
  detD[5] := FOrigin[1] - vecP1[1];
  detD[8] := FOrigin[2] - vecP1[2];
  Dlambda := CalcDeterminant(detD);
  lambda := Dlambda / DMain;

  For Index := 0 to 2 do
    vecX[Index] := FOrigin[Index] + FDir[Index] * lambda;

  detD[2] := vecNormal[0]; detD[5] := vecNormal[1]; detD[8] := vecNormal[2];
  DMain := CalcDeterminant (detD);
  Assert(DMain <> 0);

  detD[0] := vecX[0] - vecP1[0]; detD[1] := vecV[0]; detD[2] := vecNormal[0];
  detD[3] := vecX[1] - vecP1[1]; detD[4] := vecV[1]; detD[5] := vecNormal[1];
  detD[6] := vecX[2] - vecP1[2]; detD[7] := vecV[2]; detD[8] := vecNormal[2];
  Dalpha := CalcDeterminant(detD);

  detD[0] := vecU[0]; detD[1] := vecX[0] - vecP1[0]; detD[2] := vecNormal[0];
  detD[3] := vecU[1]; detD[4] := vecX[1] - vecP1[1]; detD[5] := vecNormal[1];
  detD[6] := vecU[2]; detD[7] := vecX[2] - vecP1[2]; detD[8] := vecNormal[2];
  Dbeta := CalcDeterminant(detD);

  alpha := Dalpha / DMain;
  beta := Dbeta / DMain;

  result := ((alpha >= 0.0) and (alpha <= 1.0) and
            (beta >= 0.0) and (beta <= 1.0) and (alpha <= 1.0 - beta))

end;

type
  PFaceIntersectionStruct = ^TFaceIntersectionStruct;
  TFaceIntersectionStruct = record
     Ray: TMRay;
     Intersecting: Boolean;
     Lambda: Single;
  end;


constructor TMMatrix.Create;
begin
     Identity;
end;

constructor TMMatrix.CreateFrom(Matrix: TMMatrix);
begin
   Assign(Matrix);
end;

constructor TMMatrix.CreateFromGLMatrix (AMatrix: TMatrix4f);
var
  X, Y: Integer;
begin
     For X := 0 to 3 do
        For Y := 0 to 3 do
           FFields[X, Y] := AMatrix[Y, X];
end;

destructor TMMatrix.Destroy;
begin
end;

procedure TMMatrix.Identity;
var
  Index: Integer;
begin
   Zero ();
   For Index := 0 to 3 do
      FFields[Index, Index] := 1;
end;

procedure TMMatrix.Zero;
begin
   FillChar(FFields, Sizeof(FFields), 0);
end;

constructor TMMatrix.CreateScaleMatrix (Vector: TMVector);
var
  Index: Integer;
begin
   Zero ();
   For Index := 0 to 2 do
      FFields[Index, Index] := Vector[Index];
   FFields[3, 3] := 1;
end;

constructor TMMatrix.CreateTranslationMatrix (Vector: TMVector);
begin
   Identity ();
   FFields[0, 3] := Vector[0];
   FFields[1, 3] := Vector[1];
   FFields[2, 3] := Vector[2];
end;

constructor TMMatrix.CreateRotationMatrix (AVector: TMVector; Angle: Single);
var
  V: TMVector;
  c, c1, s: Single; // Helping vars
begin
  Zero ();
  Assert(Assigned(AVector));
  V := TMVector.CreateFrom(AVector);

  try
     V.Normalize;

     c := cos (Angle);
     c1 := 1 - c;
     s := sin (Angle);

     FFields[0,0] := V[0] * V[0] * c1 + c;
     FFields[1,0] := V[0] * V[1] * c1 - V[2] * s;
     FFields[2,0] := V[0] * V[2] * c1 + V[1] * s;

     FFields[0,1] := V[1] * V[0] * c1 + V[2] * s;
     FFields[1,1] := V[1] * V[1] * c1 + c;
     FFields[2,1] := V[1] * V[2] * c1 - V[0] * s;

     FFields[0,2] := V[2] * V[0] * c1 - V[1] * s;
     FFields[1,2] := V[2] * V[1] * c1 + V[0] * s;
     FFields[2,2] := V[2] * V[2] * c1 + c;

     FFields[3,3] := 1;

  finally
     V.Free;
  end;

end;

constructor TMMatrix.CreateInversedMatrix (AMatrix: TMMatrix);
var
  Det, invDet: Single;
  d12, d13, d23, d24, d34, d41: Single;
  tmp: array[0..15] of Single;
  m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44: Single;
begin
   Assert(Assigned(AMatrix));

   m11 := AMatrix[0, 0];
   m12 := AMatrix[1, 0];
   m13 := AMatrix[2, 0];
   m14 := AMatrix[3, 0];
   m21 := AMatrix[0, 1];
   m22 := AMatrix[1, 1];
   m23 := AMatrix[2, 1];
   m24 := AMatrix[3, 1];
   m31 := AMatrix[0, 2];
   m32 := AMatrix[1, 2];
   m33 := AMatrix[2, 2];
   m34 := AMatrix[3, 2];
   m41 := AMatrix[0, 3];
   m42 := AMatrix[1, 3];
   m43 := AMatrix[2, 3];
   m44 := AMatrix[3, 3];

  d12 := (m31 * m42 - m41 * m32);
  d13 := (m31 * m43 - m41 * m33);
  d23 := (m32 * m43 - m42 * m33);
  d24 := (m32 * m44 - m42 * m34);
  d34 := (m33 * m44 - m43 * m34);
  d41 := (m34 * m41 - m44 * m31);
  tmp[0] := (m22 * d34 - m23 * d24 + m24 * d23);
  tmp[1] := -(m21 * d34 + m23 * d41 + m24 * d13);
  tmp[2] := (m21 * d24 + m22 * d41 + m24 * d12);
  tmp[3] := -(m21 * d23 - m22 * d13 + m23 * d12);

  det := m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

  if det = 0 then
     raise EMatrixError.Create('Could not inverse matrix');

    invDet := 1.0 / det;

    tmp[0] := tmp[0] * invDet;
    tmp[1] := tmp[1] * invDet;
    tmp[2] := tmp[2] * invDet;
    tmp[3] := tmp[3] * invDet;

    tmp[4] := -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
    tmp[5] := (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
    tmp[6] := -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
    tmp[7] := (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

    d12 := m11 * m22 - m21 * m12;
    d13 := m11 * m23 - m21 * m13;
    d23 := m12 * m23 - m22 * m13;
    d24 := m12 * m24 - m22 * m14;
    d34 := m13 * m24 - m23 * m14;
    d41 := m14 * m21 - m24 * m11;

    tmp[8] := (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
    tmp[9] := -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
    tmp[10] := (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
    tmp[11] := -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
    tmp[12] := -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
    tmp[13] := (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
    tmp[14] := -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
    tmp[15] := (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

    FFields[0,0] := tmp[0];
    FFields[0,1] := tmp[1];
    FFields[0,2] := tmp[2];
    FFields[0,3] := tmp[3];
    FFields[1,0] := tmp[4];
    FFields[1,1] := tmp[5];
    FFields[1,2] := tmp[6];
    FFields[1,3] := tmp[7];
    FFields[2,0] := tmp[8];
    FFields[2,1] := tmp[9];
    FFields[2,2] := tmp[10];
    FFields[2,3] := tmp[11];
    FFields[3,0] := tmp[12];
    FFields[3,1] := tmp[13];
    FFields[3,2] := tmp[14];
    FFields[3,3] := tmp[15];


end;

procedure TMMatrix.MultiplyRight (AMatrix: TMMatrix);
var
  i, j: Integer;
  NewFields: array[0..3, 0..3] of Single;
begin
   Assert(Assigned(AMatrix));
   for i := 0 to 3 do
    for j := 0 to 3 do
      NewFields[i, j] := Self.FFields[i, 0] * AMatrix.FFields[0, j] +
                              Self.FFields[i, 1] * AMatrix.FFields[1, j] +
                              Self.FFields[i, 2] * AMatrix.FFields[2, j] +
                              Self.FFields[i, 3] * AMatrix.FFields[3, j];
   for i := 0 to 3 do
    for j := 0 to 3 do
      FFields[i, j] := NewFields[i, j];
end;

procedure TMMatrix.MultiplyLeft (AMatrix: TMMatrix);
var
  tempMatrix: TMMatrix;
begin
     Assert(Assigned(AMatrix));
     tempMatrix := TMMatrix.CreateFrom(AMatrix);
     try
        tempMatrix.MultiplyRight(Self);
        Assign(tempMatrix);
     finally
        tempMatrix.Free;
     end;
end;

function TMMatrix.GetField(X, Y: Integer): Single;
begin
   if (X < 0) or (X > 3) or (Y < 0) or (Y > 3) then
      raise EMatrixError.Create('GetField out of range');
   result := FFields[X, Y];
end;

procedure TMMatrix.SetField(X, Y: Integer; Value: Single);
begin
   if (X < 0) or (X > 3) or (Y < 0) or (Y > 3) then
      raise EMatrixError.Create('SetField out of range');
   FFields[X, Y] := Value;
end;

function TMMatrix.Inverse: TMMatrix;
begin
  result := TMMatrix.CreateInversedMatrix(Self);
end;

function TMMatrix.Apply (AVector: TMVector): TMVector;
var
  Index: Integer;
begin
  Assert(Assigned(AVector));
  result := TMVector.Create;
  For Index := 0 to 2 do
      result[Index] := Fields[Index, 0] * AVector[0] +
                       Fields[Index, 1] * AVector[1] +
                       Fields[Index, 2] * AVector[2] +
                       Fields[Index, 3];
end;

function TMMatrix.ApplyRotation (AVector: TMVector): TMVector;
var
  Index: Integer;
begin
  Assert(Assigned(AVector));
  result := TMVector.Create;
  For Index := 0 to 2 do
      result[Index] := Fields[Index, 0] * AVector[0] +
                       Fields[Index, 1] * AVector[1] +
                       Fields[Index, 2] * AVector[2];
end;

function TMMatrix.Apply (ARay: TMRay): TMRay;
var
  vecTemp: TMVector;
begin
   Assert(Assigned(ARay));
   result := TMRay.Create;

   vecTemp := Apply(ARay.Origin);
   result.Origin.Assign(vecTemp);
   vecTemp.Free;

   vecTemp := ApplyRotation(ARay.Dir);
   result.Dir.Assign(vecTemp);
   vecTemp.Free;
end;

procedure TMMatrix.Assign(AMatrix: TMMatrix);
var
  X, Y: Integer;
begin
     Assert(Assigned(AMatrix));
     For X := 0 to 3 do
        For Y := 0 to 3 do
           FFields[X, Y] := AMatrix.Fields[X, Y];
end;

procedure TMMatrix.Apply (Var X, Y, Z: Single);
var
  vecTemp: TVector;
  Index: Integer;
begin
  For Index := 0 to 2 do
      vecTemp[Index] := Fields[Index, 0] * X +
                       Fields[Index, 1] * Y +
                       Fields[Index, 2] * Z +
                       Fields[Index, 3];
  X := vecTemp[0];
  Y := vecTemp[1];
  Z := vecTemp[2];
end;

procedure TMMatrix.ApplyRotation (Var X, Y, Z: Single);
var
  vecTemp: TVector;
  Index: Integer;
begin
  For Index := 0 to 2 do
      vecTemp[Index] := Fields[Index, 0] * X +
                       Fields[Index, 1] * Y +
                       Fields[Index, 2] * Z;
  X := vecTemp[0];
  Y := vecTemp[1];
  Z := vecTemp[2];
end;

end.
