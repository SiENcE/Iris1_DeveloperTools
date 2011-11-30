unit Unit_ViewPort;
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

uses Unit_OpenGL, Controls, Graphics, Classes, Unit_OpenGLWrapper, Windows, Unit_Matrix,
Unit_OpenGLGeometry;

type

TViewPortOrientation = (voCustom, voTop, voBottom, voLeft, voRight, voFront, voBack, voIso);

TViewPort = class (TObject)
   private
      FLeft, FTop, FWidth, FHeight: Integer;
      FButton: Integer;
      OldX, OldY: Integer;
      FRotateMode: Integer;
      FStartWinkel: Single;
      FAllowRotation: Boolean;
      FMatrix: TMatrix4f;
      FProjectionMatrix: TMatrix4f;
      FUpdated: Boolean;
      FViewCenter: TVector3f;
      function GetUpdated: Boolean;
   public
      property Matrix: TMatrix4f read FMatrix;
      property Updated: Boolean read GetUpdated;
      property Left: Integer read FLeft write FLeft;
      property Top: Integer read FTop write FTop;
      property Width: Integer read FWidth write FWidth;
      property Height: Integer read FHeight write FHeight;
      property AllowRotation: Boolean read FAllowRotation write FAllowRotation;
      constructor Create(ALeft, ATop, AWidth, AHeight: Integer);
      procedure ResetView;
      procedure LoadView;
      procedure MouseDown (X, Y: Integer; Button: TMouseButton);
      procedure MouseUp;
      function MouseMove(X, Y: Integer; Shift: TShiftState): TCursor;
      procedure MouseWheel (Delta: Integer);
      procedure Apply (Engine: TGLEngine; WindowHeight: Integer);
      procedure SetOrientation (Orientation: TViewPortOrientation);
      procedure Resize (AWidth, AHeight: Integer);
      procedure SetPosition (ALeft, ATop: Integer);
      function TransformCoord (X, Y, Z: Single): TPoint;
      procedure Move(X, Y, Z: Single);
      function GetPickRay (X, Y: Integer): TMRay;
      function BerechneClickWinkel(X, Y: Single; CenterX, CenterY: Single): Single;
      procedure InitOrtho;
      procedure DoneOrtho;
end;

implementation

function ApplyMatrix (Matrix: TMatrix4f; Vertex: TVector4f): TVector4f;
var
  Index: Integer;
begin
  For Index := 0 to 3 do
     result[Index] := Matrix[0, Index] * Vertex[0] +
                      Matrix[1, Index] * Vertex[1] +
                      Matrix[2, Index] * Vertex[2] +
                      Matrix[3, Index] * Vertex[3];
end;

constructor TViewPort.Create(ALeft, ATop, AWidth, AHeight: Integer);
begin
   FLeft := ALeft;
   FTop := ATop;
   Resize(AWidth, AHeight);
   FAllowRotation := True;
   FViewCenter [0] := 0;
   FViewCenter [1] := 0;
   FViewCenter [2] := 5;
   ResetView ();
end;

procedure TViewPort.ResetView;
begin
   glLoadIdentity();
   glRotatef (- 90, 1, 0, 0);
   glScalef (- 1, 1, 1.0);
   glGetFloatv(GL_MODELVIEW_MATRIX, @FMatrix[0,0]);
end;

procedure TViewPort.LoadView;
var
  AMatrix : TMatrix4f;
begin
     AMatrix := FMatrix;
     AMatrix[3,0] := AMatrix[3,0] - FViewCenter[0];
     AMatrix[3,1] := AMatrix[3,1] - FViewCenter[1];
     AMatrix[3,2] := AMatrix[3,2] - FViewCenter[2];
     glLoadMatrixf(@AMatrix[0]);
end;

procedure TViewPort.MouseDown (X, Y: Integer; Button: TMouseButton);
begin
    dec (X, FLeft);
    dec (Y, FTop);
    if (X < 0) or (Y < 0) or (X >= FWidth) or (Y >= FHeight) then exit;

    if Button = mbLeft then
       FButton := 1
    else
       FButton := 2;

    FRotateMode := 0;
    if (X < FWidth * 0.15) or (X > FWidth * 0.85) or
      (Y < FHeight *0.15) or (Y > FHeight *0.85) then begin
         FRotateMode := 1;
         FStartWinkel := BerechneClickWinkel(X, Y, FWidth / 2, FHeight / 2);
      end;
end;

procedure TViewPort.MouseUp;
begin
     FButton := 0;
end;

function TViewPort.MouseMove(X, Y: Integer; Shift: TShiftState): TCursor;
   var
     Winkel: Single;
     isLeft, isRight, isTop, isBottom: Boolean;
   begin
    result := crNone;

    dec (X, FLeft);
    dec (Y, FTop);
    if (X < 0) or (Y < 0) or (X >= FWidth) or (Y >= FHeight) then exit;

    isLeft := X < FWidth * 0.15;
    isRight := X > FWidth * 0.85;
    isTop := Y < FHeight * 0.15;
    isBottom := Y > FHeight * 0.85;

    if (isLeft and isTop) or (isRight and isBottom) then result := crSizeNESW;
    if (isLeft and isBottom) or (isRight and isTop) then result := crSizeNWSE;
    if (isLeft or isRight) and not (isTop or isBottom) then result := crSizeNS;
    if (isTop or isBottom) and not (isLeft or isRight) then result := crSizeWE;

    if (ssCtrl in Shift) and (FButton <> 0) then begin

    Move (0, 0, (Y - OldY) * 0.1);
   end else begin

   if (FButton = 2) and FAllowRotation and (not ((ssCtrl in Shift) or (ssShift in Shift))) then begin
    if FRotateMode = 0 then begin
       glLoadIdentity;
       glRotatef((X - OldX), 0,1,0);
       glMultMatrixf(@FMatrix[0,0]);
       glGetFloatv(GL_MODELVIEW_MATRIX, @FMatrix[0,0]);

       glLoadIdentity;
       glRotatef((Y - OldY), 1,0,0);
       glMultMatrixf(@FMatrix[0,0]);
       glGetFloatv(GL_MODELVIEW_MATRIX, @FMatrix[0,0]);
    end else begin
        Winkel := BerechneClickWinkel(X, Y, FWidth / 2, FHeight / 2);
        if (Winkel <> FStartWinkel) then begin
           glLoadIdentity;
           glRotatef((FStartWinkel - Winkel) / PI * 180, 0,0,1);
           glMultMatrixf(@FMatrix[0,0]);
           glGetFloatv(GL_MODELVIEW_MATRIX, @FMatrix[0,0]);
           FStartWinkel := Winkel;
        end;
    end;
  end;

  if (FButton <> 0) and (ssShift in Shift) then begin
    Move ((OldX - X) * 0.02, (Y - OldY) * 0.02, 0);
//       glLoadIdentity;
{       if (FWidth <> 0) and (FHeight <> 0) then
         glTranslatef((X - OldX) / FWidth * SizeX, - (Y - OldY) / FHeight * SizeY, 0); }
{       glMultMatrixf(@FMatrix[0,0]);
       glGetFloatv(GL_MODELVIEW_MATRIX, @FMatrix[0,0]);}
  end;

  end;

  if FButton <> 0 then
     FUpdated := True;

  OldX := X;
  OldY := Y;
end;

procedure TViewPort.MouseWheel (Delta: Integer);
begin
     Move (0, 0, Delta * 0.002);
end;

function TViewPort.BerechneClickWinkel(X, Y: Single; CenterX, CenterY: Single): Single;
var
   dx, dy: Single;
begin
      dx := X - CenterX;
      dy := Y - CenterY;
      if dx = 0 then
        dx := 0.01;
      result := arctan(dy / dx);
      if (dx < 0) and (dy > 0) then
         result := result + PI;
      if (dx < 0) and (dy < 0) then
         result := result - PI;
end;

function TViewPort.GetUpdated: Boolean;
begin
     result := FUpdated;
     FUpdated := False;
end;

procedure TViewPort.Apply (Engine: TGLEngine; WindowHeight: Integer);
begin
    Assert(Assigned(Engine));
    Engine.SetViewPort(FLeft, WindowHeight - 1 - FTop - FHeight, FWidth, FHeight);
    glGetFloatv(GL_PROJECTION_MATRIX, @FProjectionMatrix[0]);
end;

procedure TViewPort.SetOrientation (Orientation: TViewPortOrientation);
begin
     glPushMatrix ();
     glLoadIdentity ();
     case Orientation of
         voLeft: begin
                 glRotatef(90, 0, 1, 0);
                 glRotatef(- 90, 1, 0, 0);
                 end;
         voRight: begin
                 glRotatef(- 90, 0, 1, 0);
                 glRotatef(- 90, 1, 0, 0);
                 end;
         voFront: glRotatef(- 90, 1, 0, 0);
         voBack: begin
                  glRotatef(- 90, 1, 0, 0);
                  glRotatef(180, 0, 0, 1);
                 end;
         voIso: begin
                   glRotatef(- 60, 1, 0, 0);
                   glRotatef(120, 0, 0, 1);
                end;
         voBottom: glRotatef(180, 1, 0, 0);
     end;
     glScalef(-1,1,1.0);
     glGetFloatv(GL_MODELVIEW_MATRIX, @FMatrix[0,0]);
     glPopMatrix ();
end;

procedure TViewPort.Resize (AWidth, AHeight: Integer);
begin
   FWidth := AWidth;
   FHeight := AHeight;
end;

procedure TViewPort.SetPosition (ALeft, ATop: Integer);
begin
   FLeft := ALeft;
   FTop := ATop;
end;

function TViewPort.TransformCoord (X, Y, Z: Single): TPoint;
var
  vecTemp1, vecTemp2, vecTemp3: TVector4f;
  AMatrix : TMatrix4f;
begin
     AMatrix := FMatrix;
     AMatrix[3,0] := AMatrix[3,0] - FViewCenter[0];
     AMatrix[3,1] := AMatrix[3,1] - FViewCenter[1];
     AMatrix[3,2] := AMatrix[3,2] - FViewCenter[2];
     vecTemp1[0] := X;
     vecTemp1[1] := Y;
     vecTemp1[2] := Z;
     vecTemp1[3] := 1;
     vecTemp2 := ApplyMatrix(AMatrix, vecTemp1);
     vecTemp3 := ApplyMatrix(FProjectionMatrix, vecTemp2);
     if vecTemp3[3] = 0 then vecTemp3[3] := 0.00001;
     result.X := trunc((vecTemp3[0] / vecTemp3[3] + 1) * 0.5 * FWidth);
     result.Y := trunc((1 - vecTemp3[1] / vecTemp3[3]) * 0.5 * FHeight);
end;


procedure TViewPort.Move(X, Y, Z: Single);
begin
       glLoadIdentity;
       FViewCenter[0] := FViewCenter[0] + X;
       FViewCenter[1] := FViewCenter[1] + Y;
       FViewCenter[2] := FViewCenter[2] + Z;
       if FViewCenter[2] < 1 then FViewCenter[2] := 1;

//       glTranslatef(X, Y, Z);
//       glMultMatrixf(@FMatrix[0,0]);
//       glGetFloatv(GL_MODELVIEW_MATRIX, @FMatrix[0,0]);
end;

function TViewPort.GetPickRay (X, Y: Integer): TMRay;
var
   Matrix, InvMatrix: TMMatrix;
   vecTempOrigin, vecTempDir: TMVector;
   vecDir: TMVector;
   vecOrigin: TMVector;
   AMatrix : TMatrix4f;
begin
   AMatrix := FMatrix;
   AMatrix[3,0] := AMatrix[3,0] - FViewCenter[0];
   AMatrix[3,1] := AMatrix[3,1] - FViewCenter[1];
   AMatrix[3,2] := AMatrix[3,2] - FViewCenter[2];
   Matrix := TMMatrix.CreateFromGLMatrix(AMatrix);
   try
      InvMatrix := Matrix.Inverse;
      vecTempOrigin := TMVector.Create;
      vecTempDir := TMVector.Create(0,0,0);
      try
         vecTempDir[0] :=  ( ( ( 2.0 * (X - FLeft) ) / FWidth  ) - 1 ) / FProjectionMatrix[0,0];
	 vecTempDir[1] := -( ( ( 2.0 * (Y - FTop) ) / FHeight ) - 1 ) / FProjectionMatrix[1,1];
	 vecTempDir[2] :=  -1.0;
         vecDir := InvMatrix.Apply (vecTempDir);
         vecOrigin := TMVector.Create(InvMatrix [0, 3], InvMatrix [1, 3], InvMatrix [2, 3]);
         vecDir.Subtract (vecOrigin);
{          vecTempOrigin[0] := ((X - FLeft) / FWidth - 0.5) * SizeX;
          vecTempOrigin[1] := (0.5 - (Y - FTop) / FHeight ) * SizeY;
          vecTempOrigin[2] := 0;}
{          vecTempDir.Add(vecTempOrigin);
          vecOrigin := InvMatrix.Apply(vecTempOrigin);
          vecDir := InvMatrix.Apply(vecTempDir);
          vecDir.Subtract(vecOrigin);}
          try
             vecDir.Normalize;
             result := TMRay.Create;
             result.Origin.Assign(vecOrigin);
             result.Dir.Assign(vecDir);
          finally
             vecDir.Free;
             vecOrigin.Free
          end;
      finally
          vecTempOrigin.Free;
          vecTempDir.Free;
          InvMatrix.Free;
      end;
   finally
      Matrix.Free;
   end;
end;


procedure TViewPort.InitOrtho;
begin
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix ();
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix ();
  glLoadIdentity();

  glOrtho(0, FWidth, FHeight, 0, NearClipping, FarClipping);

  glMatrixMode(GL_MODELVIEW);
end;

procedure TViewPort.DoneOrtho;
begin
  glMatrixMode(GL_PROJECTION);
  glPopMatrix ();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix ();

end;

end.
