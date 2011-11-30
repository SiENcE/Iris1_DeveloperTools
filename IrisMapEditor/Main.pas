unit Main;
//
// Created by: Alexander Oster - tensor@ultima-iris.de
// Modified by: Florian Fischer^SiENcE - fischer@seabeams.de
//		Multimap support added 7.7.05
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
  Dialogs, StdCtrls, ExtCtrls, XPMan, ComCtrls, Form_Heightmap, Unit_Map,
  Menus, Registry;


type
  TMainForm = class(TForm)
    Button1: TButton;
    Panel1: TPanel;
    PaintBox1: TPaintBox;
    XPManifest1: TXPManifest;
    Edit1: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Edit2: TEdit;
    TrackBar1: TTrackBar;
    Label4: TLabel;
    Button2: TButton;
    Label5: TLabel;
    MainMenu1: TMainMenu;
    Map1: TMenuItem;
    LoadMap1: TMenuItem;
    LoadRadarcolmul1: TMenuItem;
    N1: TMenuItem;
    Exit1: TMenuItem;
    Edit3: TEdit;
    Button3: TButton;
    Label6: TLabel;
    Edit4: TEdit;
    OpenDialog1: TOpenDialog;
    OpenDialog2: TOpenDialog;
    SaveMap1: TMenuItem;
    SaveDialog1: TSaveDialog;
    procedure Button1Click(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure PaintBox1Paint(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure PaintBox1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure PaintBox1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Button2Click(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure SaveMap1Click(Sender: TObject);
  private
    { Private-Deklarationen }

    Map: TMap;
    RadarCol: TRadarCol;
    ABitmap: TBitmap;

    ZoomFactor: Integer;

    MouseDownPoint: TPoint;
    MouseDown: Boolean;
    LastX, LastY: Integer;

    MarkedRect: TRect;

  public
    { Public-Deklarationen }
  end;

var
  MainForm: TMainForm;

implementation

{$R *.dfm}



function GetUODir: String;
var
  Registry: TRegistry;
begin
  result := '';

  try
     Registry := TRegistry.Create;
     try
        Registry.RootKey := HKEY_LOCAL_MACHINE;
        if Registry.OpenKey('\SOFTWARE\Origin Worlds Online\Ultima Online Third Dawn\1.0\', false) then begin
          if Registry.ValueExists ('ExePath') then
              result := ExtractFilePath (Registry.ReadString('ExePath'));
        end;
     finally
        Registry.Free;
     end;

  except
  end;

end;


procedure TMainForm.Button1Click(Sender: TObject);
var
  V: Integer;
  mapID : Integer;
begin

   if OpenDialog1.Execute then begin
      Map.Free; Map := nil;

   mapID:=0;
   if (OpenDialog1.FileName = (GetUODir+'map0.mul')) then
    mapID:=0;
   if (OpenDialog1.FileName = (GetUODir+'map2.mul')) then
    mapID:=2;
   if (OpenDialog1.FileName = (GetUODir+'map3.mul')) then
    mapID:=3;
   if (OpenDialog1.FileName = (GetUODir+'map4.mul')) then
    mapID:=4;

      case mapID of
          0: Map := TMap.Create (OpenDialog1.FileName, mtMap0);
          2: Map := TMap.Create (OpenDialog1.FileName, mtMap2);
          3: Map := TMap.Create (OpenDialog1.FileName, mtMap3);
          4: Map := TMap.Create (OpenDialog1.FileName, mtMap4);
      end;

      ZoomFactor := trunc (Map.Width / Paintbox1.Width + 0.999);
      V := trunc (Map.Height / Paintbox1.Height + 0.999);
      if V > ZoomFactor then
          ZoomFactor := V;

      if Assigned (RadarCol) then
          Map.CreatePreview(ABitmap, ZoomFactor, RadarCol);

      Paintbox1.Refresh;
      Edit3.Text := OpenDialog1.FileName;
   end;

end;

procedure TMainForm.FormDestroy(Sender: TObject);
begin
      RadarCol.Free;
      ABitmap.Free;
      Map.Free;
end;

procedure TMainForm.PaintBox1Paint(Sender: TObject);
begin
   if Assigned (ABitmap) then
          Paintbox1.Canvas.Draw(0, 0, ABitmap);
   Paintbox1.Canvas.Pen.Mode := pmNot;
   Paintbox1.Canvas.Rectangle(MarkedRect);
end;

procedure TMainForm.FormCreate(Sender: TObject);
var
  UODir: String;
  V: Integer;
begin
   ZoomFactor := 1;
   ABitmap := TBitmap.Create;

   UODir := GetUODir;
   if UODir <> '' then begin
      try
         RadarCol := TRadarCol.Create (UODir + 'RadarCol.mul');
         Edit4.Text := UODir + 'RadarCol.mul';
         OpenDialog2.FileName := Edit3.Text;

         Map := TMap.Create (UODir + 'map0.mul', mtMap0);
         Edit3.Text := UODir + 'map0.mul';
         OpenDialog1.FileName := Edit4.Text;
         ZoomFactor := trunc (Map.Width / Paintbox1.Width + 0.999);
         V := trunc (Map.Height / Paintbox1.Height + 0.999);
         if V > ZoomFactor then
          ZoomFactor := V;

         Map.CreatePreview(ABitmap, ZoomFactor, RadarCol);
         Paintbox1.Refresh;
      except
      end;
   end;
end;

procedure TMainForm.PaintBox1MouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if Assigned (Map) then begin
    MouseDownPoint.X := X;
    MouseDownPoint.Y := Y;

    if X >= (Map.Width div ZoomFactor) then
        X := (Map.Width div ZoomFactor);
    if Y >= (Map.Height div ZoomFactor) then
        Y := (Map.Height div ZoomFactor);

    MouseDown := True;
    LastX := X; LastY := Y;
    Paintbox1.Canvas.Pen.Mode := pmNot;
    Paintbox1.Canvas.Brush.Style := bsClear;
    Paintbox1.Canvas.Rectangle(MouseDownPoint.X, MouseDownPoint.Y, LastX, LastY);
  end;
end;

procedure TMainForm.PaintBox1MouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Integer);
begin
   if MouseDown and Assigned (Map) then begin
       if X >= (Map.Width div ZoomFactor) then
          X := (Map.Width div ZoomFactor);
       if Y >= (Map.Height div ZoomFactor) then
          Y := (Map.Height div ZoomFactor);

       Paintbox1.Canvas.Pen.Mode := pmNot;
       Paintbox1.Canvas.Brush.Style := bsClear;
       Paintbox1.Canvas.Rectangle(MouseDownPoint.X, MouseDownPoint.Y, LastX, LastY);
       LastX := X; LastY := Y;
       Paintbox1.Canvas.Rectangle(MouseDownPoint.X, MouseDownPoint.Y, X, Y);

       if MouseDownPoint.X < X then begin
           MarkedRect.Left := MouseDownPoint.X;
           MarkedRect.Right := X;
       end else begin
           MarkedRect.Left := X;
           MarkedRect.Right := MouseDownPoint.X;
       end;
       if MouseDownPoint.Y < Y then begin
           MarkedRect.Top := MouseDownPoint.Y;
           MarkedRect.Bottom := Y;
       end else begin
           MarkedRect.Top := Y;
           MarkedRect.Bottom := MouseDownPoint.Y;
       end;

       Edit1.Text := Format ('%d, %d', [ MarkedRect.Top * ZoomFactor,  MarkedRect.Left * ZoomFactor]);
       Edit2.Text := Format ('%d, %d', [ MarkedRect.Bottom * ZoomFactor,  MarkedRect.Right * ZoomFactor]);


   end;
end;

procedure TMainForm.PaintBox1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
   if MouseDown and Assigned (Map) then begin
       Paintbox1.Canvas.Pen.Mode := pmNot;
       Paintbox1.Canvas.Brush.Style := bsClear;
       Paintbox1.Canvas.Rectangle(MouseDownPoint.X, MouseDownPoint.Y, LastX, LastY);
       MouseDown := False;
       Paintbox1.Refresh;
   end;
end;

procedure TMainForm.Button2Click(Sender: TObject);
var
  EditForm: TEditform;
  Rect: TRect;
begin
  Rect := MarkedRect;
  Rect.Top := Rect.Top * ZoomFactor;
  Rect.Left := Rect.Left * ZoomFactor;
  Rect.Bottom := Rect.Bottom * ZoomFactor;
  Rect.Right := Rect.Right * ZoomFactor;

  if ((Rect.Right - Rect.Left) < 32) or ((Rect.Bottom - Rect.Top) < 32) then
      raise Exception.Create ('Selection too small');

  if ((Rect.Right - Rect.Left) > 512) or ((Rect.Bottom - Rect.Top) > 512) then
      raise Exception.Create ('Selection too large');

  EditForm := TEditform.Create (Self, Map, Rect, Trackbar1.Position, RadarCol);
  EditForm.ShowModal;


end;

procedure TMainForm.Exit1Click(Sender: TObject);
begin
   Close;
end;

procedure TMainForm.Button3Click(Sender: TObject);
begin
   if OpenDialog2.Execute then begin
      Edit4.Text := OpenDialog2.FileName;
      RadarCol.Free; RadarCol := nil;
      RadarCol := TRadarCol.Create (OpenDialog2.FileName);
      if Assigned (RadarCol) then
          Map.CreatePreview(ABitmap, ZoomFactor, RadarCol);
      Paintbox1.Refresh;
   end;
end;

procedure TMainForm.SaveMap1Click(Sender: TObject);
begin
  if Assigned(Map) and SaveDialog1.Execute then begin
      Map.Save(SaveDialog1.FileName);
  end;
end;

end.
