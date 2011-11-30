unit Form_Heightmap;
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

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Unit_OpenGLGeometry, Unit_OpenGL, Unit_OpenGLWrapper, Math,
  Unit_ViewPort, Unit_Matrix, ExtCtrls, ComCtrls, XPMan, Menus, Unit_Map;

const
  UOZFactor = 0.05;

type
  PVertexArray = ^TVertexArray;
  TVertexArray = array [0..255] of TVector3f;

  PFloatArray = ^TFloatArray;
  TFloatArray = array [0..255] of Single;

  TPointRaster = class (TObject)
      private
        FArray: PVertexArray;
        FWidth, FHeight: Integer;
        procedure SetPoint (Index1, Index2: Integer; AVector: TVector3f);
        function GetPoint (Index1, Index2: Integer): TVector3f;
      public
        property Points [Index1, Index2: Integer]: TVector3f read GetPoint write SetPoint; default;
        property Width: Integer read FWidth;
        property Height: Integer read FHeight;

        constructor Create (AWidth, AHeight: Integer);
        destructor Destroy; override;
  end;

  TBezierPatch = class (TObject)
      private
         FPoints: array[0..3, 0..3] of TVector3f;
      public
         constructor Create;
         procedure CreateRaster (ARaster: TPointRaster; AX, AY: Integer; ASize: Integer);
  end;

  TBezierField = class (TObject)
      private
         FArray: PFloatArray;
         FWidth: Integer;
         FHeight: Integer;
         procedure SetPoint (Index1, Index2: Integer; AValue: Single);
         function GetPoint (Index1, Index2: Integer): Single;
      public
         property Points [Index1, Index2: Integer]: Single read GetPoint write SetPoint; default;
         constructor Create (AWidth, AHeight: Integer); // Width and Height must be even
         destructor Destroy; override;
         procedure CreateRaster (var ARaster: TPointRaster; Resolution: Integer); // Resolution:
  end;

  type
  TEditForm = class(TForm)
    Panel1: TPanel;
    Label1: TLabel;
    TrackBar1: TTrackBar;
    Label2: TLabel;
    TrackBar2: TTrackBar;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    Timer1: TTimer;
    CheckBox3: TCheckBox;
    SaveDialog1: TSaveDialog;
    XPManifest1: TXPManifest;
    MainMenu1: TMainMenu;
    Effects1: TMenuItem;
    AddNoise1: TMenuItem;
    Settings1: TMenuItem;
    ShowTileLines1: TMenuItem;
    Lighting1: TMenuItem;
    StatusBar: TStatusBar;
    procedure FormDestroy(Sender: TObject);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure FormMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormResize(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure ShowTileLines1Click(Sender: TObject);
    procedure Lighting1Click(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private-Deklarationen }
    FEngine: TGLEngine;
    FViewPort: TViewPort;
    FX, FY: Integer;
    FMarkedX, FMarkedY: Integer;
    FResolution: Integer;
    FChanging: Boolean;
    FShowTileLines: Boolean;
    FLighting: Boolean;
    AField: TBezierField;
    ARaster: TPointRaster;
    FRect: TRect;

    FMap: TMap;
    FRadarCol: TRadarCol;

    procedure Render;
    procedure ShowStatus;
  public
    { Public-Deklarationen }
    constructor Create(AOwner: TComponent; AMap: TMap; ARect: TRect; ADetail: Integer; ARadarCol: TRadarCol); reintroduce;
  end;

implementation

{$R *.dfm}


  function BernsteinTransformation (var P1, P2, P3, P4: TVector3f; t: Single): TVector3f;
  var
    one_minus_t: Single;
  begin
      one_minus_t := 1 - t;

      result := VectorScale(P1, one_minus_t * one_minus_t * one_minus_t);
      AddVector (result, VectorScale(P2, 3 * t * one_minus_t * one_minus_t));
      AddVector (result, VectorScale(P3, 3 * t * t * one_minus_t));
      AddVector (result, VectorScale(P4, t * t * t));
  end;


  constructor TPointRaster.Create (AWidth, AHeight: Integer);
  begin
     Assert (AWidth > 0);
     Assert (AHeight > 0);
     FWidth := AWidth;
     FHeight := AHeight;

     GetMem (FArray, Sizeof (TVector3f) * FWidth * FHeight);
     FillChar (FArray^, Sizeof (TVector3f) * FWidth * FHeight, 0);
  end;

  destructor TPointRaster.Destroy;
  begin
     FreeMem (FArray);
     FArray := nil;
  end;

  procedure TPointRaster.SetPoint (Index1, Index2: Integer; AVector: TVector3f);
  begin
    Assert ((Index1 >= 0) and (Index1 < FWidth));
    Assert ((Index2 >= 0) and (Index2 < FHeight));
    FArray^[Index1 + Index2 * FWidth] := AVector;
  end;

  function TPointRaster.GetPoint (Index1, Index2: Integer): TVector3f;
  begin
    Assert ((Index1 >= 0) and (Index1 < FWidth));
    Assert ((Index2 >= 0) and (Index2 < FHeight));
    result := FArray^[Index1 + Index2 * FWidth];
  end;

  constructor TBezierPatch.Create;
  var
    I, J: Integer;
  begin
     FillChar (FPoints, sizeof (FPoints), 0);
     For I := 0 to 3 do
        For J := 0 to 3 do begin
            FPoints [I, J][0] := I;
            FPoints [I, J][1] := J;
        end;
            FPoints [2, 2][2] := 6;
            FPoints [0, 2][2] := 3;
            FPoints [1, 2][2] := 5;

  end;

  procedure TBezierPatch.CreateRaster (ARaster: TPointRaster; AX, AY: Integer; ASize: Integer);
  var
    Index1, Index2, I: Integer;
    Size: Integer;
    u, v: Single;
    TempVecs: array[0..3] of TVector3f;
  begin
     Assert (Assigned (ARaster));
     Assert (ASize > 3);
     Assert ((AX >= 0) and (AY >= 0));
     Assert ((AX + ASize <= ARaster.Width) and (AY + ASize <= ARaster.Height));
     Size := ASize - 1;

     For Index1 := 0 to Size do begin
        u := Index1 / Size;

        For I := 0 to 3 do
          TempVecs[I] := BernsteinTransformation (FPoints[I, 0], FPoints[I, 1], FPoints[I, 2], FPoints[I, 3], u);

        For Index2 := 0 to Size do begin
          v := Index2 / Size;
          ARaster [Index2 + AX, Index1 + AY] := BernsteinTransformation (TempVecs[0], TempVecs[1], TempVecs[2], TempVecs[3], v);
        end;
     end;

  end;

  constructor TBezierField.Create (AWidth, AHeight: Integer);
  begin
     Assert (AWidth > 0);
     Assert (AHeight > 0);
     FWidth := AWidth * 2; // Two Control Points by row
     FHeight := AHeight * 2; // Two Control Points by col

     GetMem (FArray, Sizeof (Single) * FWidth * FHeight);
     FillChar (FArray^, Sizeof (Single) * FWidth * FHeight, 0);
  end;

  destructor TBezierField.Destroy;
  begin
    FreeMem (FArray);
    FArray := nil;
  end;

  procedure TBezierField.CreateRaster (var ARaster: TPointRaster; Resolution: Integer); // Resolution:
  const
    Coords: array[0..3] of Single = (0.0, 0.25, 0.75, 1.0);
  var
    APatch: TBezierPatch;
    X, Y, I, J: Integer;
    W, H, PX, PY: Integer;
  begin
      W := FWidth div 2;
      H := FHeight div 2;

{      For X := 0 to FWidth - 1 do begin
           Points [X, 0] := 0;
           Points [X, FHeight - 1] := 0;
      end;

      For Y := 0 to FHeight - 1 do begin
           Points [0, Y] := 0;
           Points [FWidth - 1, Y] := 0;
      end;}

      ARaster := TPointRaster.Create(Resolution * W + 1, Resolution * H + 1);

      APatch := TBezierPatch.Create;
      try
          For X := 0 to W - 1 do
             For Y := 0 to H - 1 do begin

                For I := 0 to 3 do
                   For J := 0 to 3 do begin
                      APatch.FPoints[I, J, 0] := Coords[I] + X;
                      APatch.FPoints[I, J, 1] := Coords[J] + Y;
                      APatch.FPoints[I, J, 2] := 0;
                   end;

                PX := X * 2; PY := Y * 2;

                APatch.FPoints[1, 1, 2] := Points[PX, PY];
                APatch.FPoints[1, 2, 2] := Points[PX, PY + 1];
                APatch.FPoints[2, 2, 2] := Points[PX + 1, PY + 1];
                APatch.FPoints[2, 1, 2] := Points[PX + 1, PY];

                if (X > 0) then begin
                    APatch.FPoints [0, 1, 2] := (Points [PX, PY] + Points [PX - 1, PY]) / 2;
                    APatch.FPoints [0, 2, 2] := (Points [PX, PY + 1] + Points [PX - 1, PY + 1]) / 2;
                end;

                if (X < W - 1) then begin
                    APatch.FPoints [3, 1, 2] := (Points [PX + 1, PY] + Points [PX + 2, PY]) / 2;
                    APatch.FPoints [3, 2, 2] := (Points [PX + 1, PY + 1] + Points [PX + 2, PY + 1]) / 2;
                end;

                if (Y > 0) then begin
                    APatch.FPoints [1, 0, 2] := (Points [PX, PY] + Points [PX, PY - 1]) / 2;
                    APatch.FPoints [2, 0, 2] := (Points [PX + 1, PY] + Points [PX + 1, PY - 1]) / 2;
                end;

                if (Y < H - 1) then begin
                    APatch.FPoints [1, 3, 2] := (Points [PX, PY + 1] + Points [PX, PY + 2]) / 2;
                    APatch.FPoints [2, 3, 2] := (Points [PX + 1, PY + 1] + Points [PX + 1, PY + 2]) / 2;
                end;

                if (X > 0) and (Y > 0) then
                    APatch.FPoints [0, 0, 2] := (Points [PX, PY] + Points [PX - 1, PY - 1] + Points [PX - 1, PY] + Points [PX, PY - 1]) / 4;

                if (X < W - 1) and (Y < H - 1) then
                    APatch.FPoints [3, 3, 2] := (Points [PX + 1, PY + 1] + Points [PX + 2, PY + 2] + Points [PX + 1, PY + 2] + Points [PX + 2, PY + 1]) / 4;

                if (X > 0) and (Y < H - 1) then
                    APatch.FPoints [0, 3, 2] := (Points [PX, PY + 1] + Points [PX - 1, PY + 2] + Points [PX - 1, PY + 1] + Points [PX, PY + 2]) / 4;

                if (X < W - 1) and (Y > 0) then
                    APatch.FPoints [3, 0, 2] := (Points [PX + 1, PY] + Points [PX + 2, PY - 1] + Points [PX + 2, PY] + Points [PX + 1, PY - 1]) / 4;

                APatch.CreateRaster(ARaster, X * Resolution, Y * Resolution, Resolution + 1);

             end;
      finally
        APatch.Free;
      end;
  end;

  procedure TBezierField.SetPoint (Index1, Index2: Integer; AValue: Single);
  begin
    Assert ((Index1 >= 0) and (Index1 < FWidth));
    Assert ((Index2 >= 0) and (Index2 < FHeight));
    FArray^[Index1 + Index2 * FWidth] := AValue;
  end;

  function TBezierField.GetPoint (Index1, Index2: Integer): Single;
  begin
    Assert ((Index1 >= 0) and (Index1 < FWidth));
    Assert ((Index2 >= 0) and (Index2 < FHeight));
    result := FArray^[Index1 + Index2 * FWidth];
  end;


constructor TEditForm.Create(AOwner: TComponent; AMap: TMap; ARect: TRect; ADetail: Integer; ARadarCol: TRadarCol);
begin
   inherited Create (AOwner);

   Assert (Assigned (AMap));

   Randomize;
   FResolution := ADetail * 4;
   FShowTileLines := False;
   FLighting := False;

   FEngine := TGLEngine.Create;
   AField := TBezierField.Create ((ARect.Right - ARect.Left) div ADetail div 4, (ARect.Bottom - ARect.Top) div ADetail div 4);

   AField.CreateRaster(ARaster, FResolution);

   FMap := AMap;
   FRadarCol := ARadarCol;
   FRect := ARect;

   ShowStatus;
end;

procedure TEditForm.FormDestroy(Sender: TObject);
begin
   ARaster.Free;
   ARaster := nil;
   AField.Free;
   AField := nil;
   FViewPort.Free;
   FViewPort := nil;
   FEngine.Free;
   FEngine := nil;
end;

procedure TEditForm.Render;
var
  X, Y: Integer;
  P1, P2, P3, P4, Normal: TVector3f;
  Ray: TMRay;
  MoveX, MoveY: Single;
  MarkedX, MarkedY: Integer;
  curlambda, lambda: Single;
  Color: TColor32;
  Cell: TMapCell;
  ZFactor: Single;
begin
    if not Assigned(FViewPort) or not Assigned(FEngine) then exit;
    if not Assigned (ARaster) then exit;


//    FEngine.MakeCurrent (Canvas.Handle);
    FViewPort.Apply(FEngine, ClientHeight);

    glClear(GL_DEPTH_BUFFER_BIT or GL_COLOR_BUFFER_BIT);
    glEnable (GL_DEPTH_TEST);
    FViewPort.LoadView;


    MoveX := - ARaster.Width / FResolution / 2;
    MoveY := - ARaster.Height / FResolution / 2;

    glPushMatrix ();
    glTranslatef(MoveX, MoveY, 0);

    glDisable(GL_LIGHTING);
    glColor3f(0.5, 0.5, 0.5);
    if FShowTileLines then begin
    For X := 0 to ARaster.Width - 1 do begin
      glBegin (GL_LINE_STRIP);
      For Y := 0 to ARaster.Height - 1 do begin
          P1 := ARaster[X, Y];
          glVertex3fv (@P1[0]);
      end;
      glEnd ();
    end;

    For Y := 0 to ARaster.Height - 1 do begin
      glBegin (GL_LINE_STRIP);
      For X := 0 to ARaster.Width - 1 do begin
          P1 := ARaster[X, Y];
          glVertex3fv (@P1[0]);
      end;
      glEnd ();
    end;
    end;
    glPopMatrix ();

    Ray := FViewPort.GetPickRay(FX, FY);
    try

    MarkedX := -1; MarkedY := -1; curlambda := 100000;
    if FLighting then
        glEnable(GL_LIGHTING);
    glColor3f(0.8, 0.8, 0.9);
    glBegin (GL_QUADS);
    For Y := 0 to ARaster.Height - 2 do begin
      For X := 0 to ARaster.Width - 2 do begin
          P1 := ARaster[X, Y];
          P2 := ARaster[X, Y + 1];
          P3 := ARaster[X + 1, Y + 1];
          P4 := ARaster[X + 1, Y];

          Cell := FMap.GetCell(FRect.Left + X, FRect.Top + Y);
          Color := FRadarCol.Colors[Cell.TileID];
          glColor3f (Color.r / 255, Color.g / 255, Color.b / 255);

          ZFactor := 0.1 / FResolution;
          AddVector (P1, AffineVectorMake(MoveX, MoveY, Cell.Z * ZFactor));
          AddVector (P2, AffineVectorMake(MoveX, MoveY, FMap.GetCell(FRect.Left + X, FRect.Top + Y + 1).Z  * ZFactor));
          AddVector (P3, AffineVectorMake(MoveX, MoveY, FMap.GetCell(FRect.Left + X + 1, FRect.Top + Y + 1).Z  * ZFactor));
          AddVector (P4, AffineVectorMake(MoveX, MoveY, FMap.GetCell(FRect.Left + X + 1, FRect.Top + Y).Z  * ZFactor));

          Normal := VectorCrossProduct (VectorSubtract(P3, P2), VectorSubtract(P4, P2));
          NormalizeVector (Normal);

          glNormal3fv(@Normal[0]);
          glVertex3fv (@P1[0]);
          glVertex3fv (@P2[0]);
          glVertex3fv (@P3[0]);
          glVertex3fv (@P4[0]);

          if Ray.InterSectWithTriangle(P1, P2, P3, lambda) then
            if lambda < curlambda then begin
                MarkedX := X; MarkedY := Y;
                curlambda := lambda;
             end;

          if Ray.InterSectWithTriangle(P3, P4, P1, lambda) then
            if lambda < curlambda then begin
                MarkedX := X; MarkedY := Y;
                curlambda := lambda;
            end;

      end;
    end;
    glEnd ();

    glDisable(GL_LIGHTING);

    if (MarkedX <> -1) then begin
          P1 := ARaster[MarkedX, MarkedY];
          P2 := ARaster[MarkedX, MarkedY + 1];
          P3 := ARaster[MarkedX + 1, MarkedY + 1];
          P4 := ARaster[MarkedX + 1, MarkedY];

          AddVector (P1, P2);
          AddVector (P1, P3);
          AddVector (P1, P4);
          ScaleVector (P1, 0.25);
          AddVector (P1, AffineVectorMake(MoveX, MoveY, 0));

          glColor3f(1,0,0);
          glBegin (GL_TRIANGLES);
          glVertex3f (P1[0], P1[1] - 0.0577, P2[2] + 0.1);
          glVertex3f (P1[0] - 0.05, P1[1] + 0.0228, P2[2] + 0.1);
          glVertex3f (P1[0] + 0.05, P1[1] + 0.0228, P2[2] + 0.1);
          glVertex3f (P1[0] + 0.05, P1[1] + 0.0228, P2[2] + 0.1);
          glVertex3f (P1[0] - 0.05, P1[1] + 0.0228, P2[2] + 0.1);
          glVertex3f (P1[0], P1[1], P2[2]);
          glVertex3f (P1[0], P1[1], P2[2]);
          glVertex3f (P1[0] - 0.05, P1[1] + 0.0228, P2[2] + 0.1);
          glVertex3f (P1[0], P1[1] - 0.0577, P2[2] + 0.1);

          glVertex3f (P1[0], P1[1] - 0.0577, P2[2] + 0.1);
          glVertex3f (P1[0] + 0.05, P1[1] + 0.0228, P2[2] + 0.1);
          glVertex3f (P1[0], P1[1], P2[2]);
          glEnd ();

          FMarkedX := round (MarkedX / (FResolution / 2));
          FMarkedY := round (MarkedY / (FResolution / 2));
          if FMarkedX >= AField.FWidth then FMarkedX := AField.FWidth - 1;
          if FMarkedY >= AField.FHeight then FMarkedY := AField.FHeight - 1;

    end else begin
        FMarkedX := -1;
        FMarkedY := -1;
    end;

    finally
      Ray.Free;
    end;

    glDisable (GL_TEXTURE_2D);

    SwapBuffers (Canvas.Handle);
end;

procedure TEditForm.FormMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
       FViewPort.MouseDown(X, Y, Button);
       if (Button = mbLeft) and not (ssAlt in Shift) and (FMarkedX <> -1) then begin
          FChanging := True;
       end;

end;

procedure TEditForm.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
   FX := X; FY := Y;
   if Assigned (FViewPort) then begin
       FViewPort.MouseMove(X, Y, Shift);
//   if FViewPort.Updated then
       if not FChanging then
          Render;
end;

end;

procedure TEditForm.FormMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if Assigned (FViewPort) then
   FViewPort.MouseUp;
   FChanging := False;

end;

procedure TEditForm.FormResize(Sender: TObject);
begin
   if Assigned (FViewPort) then begin
      FViewPort.Resize(ClientWidth, ClientHeight - Panel1.Height);
      FViewPort.SetPosition(0, 0);
      Render;
   end;

end;

procedure TEditForm.FormPaint(Sender: TObject);
begin
  if not Assigned (FViewPort) then begin
     FEngine.StartGL (Canvas.Handle);
     FEngine.SetupGL;


     FViewPort := TViewPort.Create(0, 0, ClientWidth, ClientHeight);
     FViewPort.SetOrientation(voIso);
  end;

  Render;
end;

procedure TEditForm.Timer1Timer(Sender: TObject);
var
  Amount: Single;
  PenSize: Single;
  AX, AY: Integer;
  D, F: Single;
begin
    if FChanging and (FMarkedX <> -1) then begin
          Amount := Trackbar1.Position * 0.003;
          PenSize := Trackbar2.Position / 2;
          if RadioButton2.Checked then Amount := - Amount;
          For AX := FMarkedX - trunc(PenSize - 1) to FMarkedX + trunc(PenSize + 1) do
          For AY := FMarkedY - trunc(PenSize - 1) to FMarkedY + trunc(PenSize + 1) do
              if (AX >= 0) and (AY >= 0) and (AX < AField.FWidth) and (AY < AField.FHeight) then begin
                  D := (FMarkedX - AX) * (FMarkedX - AX) + (FMarkedY - AY) * (FMarkedY - AY);
                  if D < PenSize * PenSize then begin
                      if Checkbox3.Checked and (D > 0) then
                          F := PenSize * PenSize / D
                      else
                         F := 1.0;
                      if F > 1.0 then F := 1.0;
                      AField[AX, AY] := AField[AX, AY] + F * Amount;
                  end;

                end;

          ARaster.Free;
          AField.CreateRaster(ARaster, FResolution);
          Render;
    end;

end;

type
  TMapCell = packed record
      Tile: Word;
      Z: ShortInt;
  end;

  TMapBlock = packed record
      Unknown: Cardinal;
      Cells: array[0..7, 0..7] of TMapCell;
  end;

procedure TEditForm.Button1Click(Sender: TObject);
var
  ARaster: TPointRaster;
  X, Y, Z, BlockX, BlockY, AX, AY: Integer;
  ARow: array[0..511] of TMapBlock;
  AStream: TStream;
begin
  if SaveDialog1.Execute then begin
      AStream := TFileStream.Create (SaveDialog1.FileName, fmCreate);
      try
        AField.CreateRaster (ARaster, 8);
        try
          For BlockX := 0 to 767 do begin
            FillChar (ARow, sizeof (ARow), 0);
            For BlockY := 0 to 511 do begin


                For X := 0 to 7 do
                    For Y := 0 to 7 do begin
                        AX := BlockX * 8 + X;
                        AY := BlockY * 8 + Y;
                        if (AX < ARaster.Width) and (AY < ARaster.Height) then begin
                            ARow[BlockY].Cells[Y, X].Tile := 3;
                            Z := trunc (ARaster[AX, AY][2] / UOZFactor);
                            if Z < -126 then Z := -126;
                            if Z > 126 then Z := 126;
                            ARow[BlockY].Cells[Y, X].Z := Z;
                        end;
                    end;

            end;
            AStream.Write(ARow, Sizeof (ARow));
          end;

        finally
            ARaster.Free;
        end;
      finally
          AStream.Free;
      end;
  end;
end;

procedure TEditForm.Button2Click(Sender: TObject);
var
 S: String;
 Amount: Integer;
 X, Y: Integer;
begin
  S := InputBox ('Add Noise', 'Amount (1-10)', '');
  Amount := StrToIntDef (S, 0);
  if (Amount >= 1) and (Amount <= 10) then begin
      For X := 0 to AField.FWidth - 1 do
        For Y := 0 to AField.FHeight - 1 do begin
            AField[X, Y] := AField[X, Y] + (Random (200) - 100) * 0.0005 * Amount;
        end;
      ARaster.Free;
      AField.CreateRaster(ARaster, FResolution);
      Render;
  end;
end;

procedure TEditForm.ShowTileLines1Click(Sender: TObject);
begin
   ShowTileLines1.Checked := not ShowTileLines1.Checked;
   FShowTileLines := ShowTileLines1.Checked;
   Render;
end;

procedure TEditForm.Lighting1Click(Sender: TObject);
begin
   Lighting1.Checked := not Lighting1.Checked;
   FLighting := Lighting1.Checked;
   Render;

end;

procedure TEditForm.ShowStatus;
begin
 if Assigned (ARaster) then
    StatusBar.Panels[1].Text := Format ('Tiles: %d x %d', [ ARaster.Width - 1, ARaster.Height - 1 ]);
  StatusBar.Panels[0].Text := Format ('Control Points: %d x %d', [ AField.FWidth div 2, AField.FHeight div 2]);
end;

procedure TEditForm.Exit1Click(Sender: TObject);
begin
   Close;
end;

procedure TEditForm.FormClose(Sender: TObject; var Action: TCloseAction);
var
  X, Y: Integer;
  ZFactor: Single;
begin
   ZFactor := 0.1 / FResolution;
   if MessageDlg ('Save Changes?', mtConfirmation, [ mbYes, mbNo ], 0) = mrYes then begin
    For Y := 0 to ARaster.Height - 2 do
      For X := 0 to ARaster.Width - 2 do begin
          FMap.IncreaseZ(FRect.Left + X, FRect.Top + Y, round(ARaster[X, Y][2] / ZFactor));
      end;
   end;
   Action := caFree;
end;


end.

