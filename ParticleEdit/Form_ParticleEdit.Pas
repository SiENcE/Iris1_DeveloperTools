unit Form_ParticleEdit;
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
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs, Unit_OpenGLWrapper,
  Unit_OpenGL, StdCtrls, Menus, ExtCtrls, ComCtrls, Unit_Particles, Unit_OpenGLGeometry,
  AppEvnts, Form_EditRotation, Form_EditTranslation, pngImage, Unit_Settings;

type
  TParticleEditForm = class(TForm)
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Edit1: TEdit;
    Label2: TLabel;
    Edit3: TEdit;
    Edit5: TEdit;
    Label5: TLabel;
    Edit6: TEdit;
    Edit7: TEdit;
    Label6: TLabel;
    Edit8: TEdit;
    Label7: TLabel;
    Edit9: TEdit;
    Edit12: TEdit;
    Label10: TLabel;
    Label11: TLabel;
    Edit13: TEdit;
    Label12: TLabel;
    Edit14: TEdit;
    Label13: TLabel;
    ComboBox1: TComboBox;
    Label14: TLabel;
    Edit15: TEdit;
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    SaveDefinitionFile1: TMenuItem;
    N1: TMenuItem;
    Exit1: TMenuItem;
    Particles1: TMenuItem;
    EditTypeDefinitions1: TMenuItem;
    GroupBox2: TGroupBox;
    Label15: TLabel;
    Edit16: TEdit;
    ListView2: TListView;
    Label19: TLabel;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    Edit17: TEdit;
    Label20: TLabel;
    ApplicationEvents1: TApplicationEvents;
    SaveDialog: TSaveDialog;
    Label3: TLabel;
    Edit2: TEdit;
    Label4: TLabel;
    Edit4: TEdit;
    Button4: TButton;
    Label9: TLabel;
    Label22: TLabel;
    Edit24: TEdit;
    Label23: TLabel;
    Edit25: TEdit;
    Label8: TLabel;
    Label21: TLabel;
    Edit26: TEdit;
    UpDown: TUpDown;
    Label16: TLabel;
    Label17: TLabel;
    Label18: TLabel;
    Label24: TLabel;
    Label25: TLabel;
    Label26: TLabel;
    Label27: TLabel;
    Button2: TButton;
    Button3: TButton;
    Button8: TButton;
    PopupMenu1: TPopupMenu;
    Rotation1: TMenuItem;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    ListView1: TListView;
    Button1: TButton;
    Button5: TButton;
    Button6: TButton;
    Button7: TButton;
    Label28: TLabel;
    Circle1: TMenuItem;
    Sphere1: TMenuItem;
    CheckBox3: TCheckBox;
    Label29: TLabel;
    Edit10: TEdit;
    Label30: TLabel;
    Edit11: TEdit;
    Label31: TLabel;
    Edit18: TEdit;
    Label32: TLabel;
    Settings1: TMenuItem;
    SetBackgroundColor1: TMenuItem;
    ColorDialog1: TColorDialog;
    Translation1: TMenuItem;
    Button9: TButton;
    CheckBox4: TCheckBox;
    Label33: TLabel;
    Label34: TLabel;
    Label35: TLabel;
    Edit19: TEdit;
    procedure FormShow(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure ApplicationEvents1Idle(Sender: TObject; var Done: Boolean);
    procedure Edit1Change(Sender: TObject);
    procedure Edit1KeyPress(Sender: TObject; var Key: Char);
    procedure Edit16Change(Sender: TObject);
    procedure ListView2Change(Sender: TObject; Item: TListItem;
      Change: TItemChange);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure EditTypeDefinitions1Click(Sender: TObject);
    procedure SaveDefinitionFile1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Rotation1Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure ListView1DblClick(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure ListView1Change(Sender: TObject; Item: TListItem;
      Change: TItemChange);
    procedure Button3Click(Sender: TObject);
    procedure FormMouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure SetBackgroundColor1Click(Sender: TObject);
    procedure Button9Click(Sender: TObject);
  private
    { Private-Deklarationen }
    GLEngine: TGLEngine;
    FButton: Boolean;
    RotateMode: Integer;
    StartWinkel: Single;
    OldX, OldY: Integer;
    ModelView: TMatrix4f;
    FZoom: Single;

    Inititalised: Boolean;


    FEffectDefinition: TEffectDefinition;
    FParticleHandler: TParticleHandler;

    function BerechneClickWinkel(X, Y: Single): Single;
    procedure Update_TypeList;
    procedure Update_ParticleList;
    procedure SetEnable;
    procedure ShowEffect;
    procedure ShowEmissions;
  public
    { Public-Deklarationen }
    constructor Create (AOwner: TComponent; AEffect: TEffectDefinition); reintroduce;
  end;


implementation

uses Form_Textures;

{$R *.DFM}

constructor TParticleEditForm.Create (AOwner: TComponent; AEffect: TEffectDefinition);
begin
  inherited Create (AOwner);
  Assert (Assigned (AEffect));
  FEffectDefinition := AEffect;
  FEffectDefinition.Lock;

  FParticleHandler := TParticleHandler.Create;

   FParticleHandler.AddEffect(FEffectDefinition, 0, 0, 0);
   FZoom := 5;

     GLEngine := TGLEngine.Create;
     Inititalised := False;
end;

procedure TParticleEditForm.FormDestroy(Sender: TObject);
begin
 FParticleHandler.Free;
 FParticleHandler := nil;
 Inititalised := False;
end;

procedure TParticleEditForm.FormResize(Sender: TObject);
begin
  if Assigned (GLEngine) and Inititalised then begin
      GLEngine.MakeCurrent(Canvas.Handle);
      GLEngine.SetViewPort(0,ClientHeight - Groupbox2.Top + 4, Groupbox1.Left - 4, Groupbox2.Top - 4);
  end;
end;

procedure TParticleEditForm.FormPaint(Sender: TObject);
var
  PNG: TPNGObject;
begin

   if not Inititalised then begin

     GLEngine.StartGL(Canvas.Handle);
     GLEngine.SetupGL;
     glLoadIdentity;
     glGetFloatv(GL_MODELVIEW_MATRIX, @ModelView[0,0]);
     GLEngine.SetViewPort(0,ClientHeight - Groupbox2.Top + 4, Groupbox1.Left - 4, Groupbox2.Top - 4);

     GLEngine.SetClearColor (Setting_BackgroundColor);
     FormResize (Sender);
     Inititalised := True;

  try
       if FileExists (Setting_TextureDir + FEffectDefinition.TextureFileName) then begin
         PNG := TPNGObject.Create;
         try
            PNG.LoadFromFile(Setting_TextureDir + FEffectDefinition.TextureFileName);
            FParticleHandler.LoadTexture(PNG);
         finally
            PNG.Free;
         end;
       end else begin
          if FEffectDefinition.TextureFileName <> '' then
             MessageDlg ('Texture "' + FEffectDefinition.TextureFileName+ '" not found!', mtError, [mbOK], 0);
       end;
  except
  end;

end;
  SwapBuffers(Canvas.Handle);

end;

procedure TParticleEditForm.Exit1Click(Sender: TObject);
begin
  Close;
end;


procedure TParticleEditForm.ApplicationEvents1Idle(Sender: TObject;
  var Done: Boolean);
begin

  if not Assigned (GLEngine) or not Inititalised then exit;

  FParticleHandler.Handle;

  GLEngine.MakeCurrent(Canvas.Handle);

  glClear(GL_DEPTH_BUFFER_BIT or GL_COLOR_BUFFER_BIT);
  glLoadIdentity;
  gltranslatef(0, 0, -FZoom);
  glMultMatrixf(@ModelView[0,0]);
  FParticleHandler.Render;
  SwapBuffers(Canvas.Handle);

  Label34.Caption := IntToStr (FParticleHandler.ParticleCount);
  Done := False;
end;


procedure TParticleEditForm.Update_TypeList;
var
   Index: Integer;
   ParticleType: TParticleTypeDefinition;
   ID: Cardinal;
begin
     ID := 0;
     if Combobox1.ItemIndex <> -1 then
        ID := Cardinal(Combobox1.Items.Objects[Combobox1.ItemIndex]);
     Combobox1.Items.Clear;
     For Index := 0 to FEffectDefinition.TypeCount - 1 do begin
         ParticleType := FEffectDefinition.Types[Index];
         Combobox1.Items.AddObject(ParticleType.Description, Pointer(ParticleType.ID));
         if ParticleType.ID = ID then
            Combobox1.ItemIndex := Index;
     end;
end;


procedure TParticleEditForm.Edit1Change(Sender: TObject);
var
   ParticleDefinition: TParticleDefinition;
begin
   if Assigned (ListView2.Selected) then begin
      ParticleDefinition := ListView2.Selected.Data;
      Assert(Assigned(ParticleDefinition));
      with ParticleDefinition do begin
         try
           if Sender = Edit1 then Speed.Angle1 := StrToFloat(Edit1.Text);
           if Sender = Edit3 then Speed.Angle2 := StrToFloat(Edit3.Text);
           if Sender = Edit2 then Speed.ConeAngle := StrToFloat(Edit2.Text);
           if Sender = Edit5 then Speed.Amount.Average := StrToFloat(Edit5.Text);
           if Sender = Edit6 then Speed.Amount.Variance := StrToFloat(Edit6.Text);
           if Sender = Edit12 then Acceleration.Angle1 := StrToFloat(Edit12.Text);
           if Sender = Edit4 then Acceleration.ConeAngle := StrToFloat(Edit4.Text);
           if Sender = Edit9 then Acceleration.Angle2 := StrToFloat(Edit9.Text);
           if Sender = Edit8 then Acceleration.Amount.Average := StrToFloat(Edit8.Text);
           if Sender = Edit7 then Acceleration.Amount.Variance := StrToFloat(Edit7.Text);
           if Sender = Edit13 then LifeTime.Average := StrToFloat(Edit13.Text);
           if Sender = Edit14 then LifeTime.Variance := StrToFloat(Edit14.Text);
           if Sender = Edit10 then Size.Average := StrToFloat(Edit10.Text);
           if Sender = Edit11 then Size.Variance := StrToFloat(Edit11.Text);
           if Sender = Edit24 then
                ParticleDefinition.AppearTime.Average := StrToIntDef(Edit24.Text, 0);
           if Sender = Edit25 then
                ParticleDefinition.AppearTime.Variance := StrToIntDef(Edit25.Text, 0);
           if (Sender = Edit24) or (Sender = Edit25) then
              ListView2.Selected.SubItems[1] := Format ('%.0f +/- %.0f ms', [ AppearTime.Average, AppearTime.Variance ]);
           if Sender = Edit26 then begin
              Amount := Updown.Position;
              ListView2.Selected.SubItems[2] := IntToStr(Amount);
           end;
           if Sender = Edit15 then begin
              Description := Edit15.Text;
              ListView2.Selected.SubItems[0] := Edit15.Text;
           end;
           if Sender = Combobox1 then begin
              if Combobox1.ItemIndex <> -1 then
                 ParticleType := Cardinal(Combobox1.Items.Objects[Combobox1.ItemIndex]);
           end;
           if Sender = Checkbox1 then
               TransformSpeed := Checkbox1.Checked;
           if Sender = Checkbox2 then
               TransformAcceleration := Checkbox2.Checked;
           if Sender = Checkbox3 then
               FadeOut := Checkbox3.Checked;
           if Sender = Checkbox4 then
               FadeIn := Checkbox4.Checked;
         except
              On E:EConvertError do
                 if (Sender as TEdit).Text <> '' then begin
                    MessageDlg('Please enter a valid value', mtError, [mbOK], 0);
                 end;
         end;
      end;

   end;
end;

procedure TParticleEditForm.Edit1KeyPress(Sender: TObject; var Key: Char);
begin
   if Key = ',' then Key := '.';
   if (not (Key in ['0'..'9', #8, '.'])) then
      Key := #0;
end;

procedure TParticleEditForm.Edit16Change(Sender: TObject);
begin
      if Sender = Edit16 then
          FEffectDefinition.Description := Edit16.Text;
      if Sender = Edit18 then
          FEffectDefinition.Name := Edit18.Text;
      if (Sender = RadioButton1) and Radiobutton1.Checked then
         FEffectDefinition.AppearType := atPeriodical;
      if (Sender = RadioButton2) and Radiobutton2.Checked then
         FEffectDefinition.AppearType := atOnce;
      if Sender = Edit17 then
         FEffectDefinition.Time := StrToIntDef(Edit17.Text, 0);
      if Sender = Edit19 then
         FEffectDefinition.EffectID := StrToIntDef(Edit19.Text, 0);
end;

procedure TParticleEditForm.ListView2Change(Sender: TObject; Item: TListItem;
  Change: TItemChange);
var
   ParticleDefinition: TParticleDefinition;
   Index: Integer;
begin
  if Assigned (ListView2.Selected) then begin
      ParticleDefinition := ListView2.Selected.Data;
      Assert(Assigned(ParticleDefinition));
      with ParticleDefinition do begin
         Edit15.Text := Description;
         Edit1.Text := Format('%.1f', [ Speed.Angle1 ]);
         Edit3.Text := Format('%.1f', [ Speed.Angle2]);
         Edit2.Text := Format('%.1f', [ Speed.ConeAngle ]);
         Edit5.Text := Format('%.1f', [ Speed.Amount.Average ]);
         Edit6.Text := Format('%.1f', [ Speed.Amount.Variance ]);
         Edit12.Text := Format('%.1f', [ Acceleration.Angle1]);
         Edit9.Text := Format('%.1f', [ Acceleration.Angle2 ]);
         Edit4.Text := Format('%.1f', [ Acceleration.ConeAngle ]);
         Edit8.Text := Format('%.1f', [ Acceleration.Amount.Average ]);
         Edit7.Text := Format('%.1f', [ Acceleration.Amount.Variance ]);
         Edit13.Text := Format('%.1f', [ Lifetime.Average ]);
         Edit14.Text := Format('%.1f', [ Lifetime.Variance ]);
         Edit24.Text := Format('%.0f', [ Appeartime.Average ]);
         Edit25.Text := Format('%.0f', [ Appeartime.Variance ]);
         Edit10.Text := Format('%.0f', [ Size.Average ]);
         Edit11.Text := Format('%.0f', [ Size.Variance ]);
         Checkbox1.Checked := TransformSpeed;
         Checkbox2.Checked := TransformAcceleration;
         Checkbox3.Checked := FadeOut;
         Checkbox4.Checked := FadeIn;
         Updown.Position := Amount;
         For Index := 0 to Combobox1.Items.Count - 1 do
             if ParticleType = Integer(Combobox1.Items.Objects[Index]) then begin
                Combobox1.ItemIndex := Index;
                break;
             end;
             ShowEmissions;
      end;
  end;
  SetEnable;

end;

procedure TParticleEditForm.FormMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
 FButton := True;
 MouseCapture := True;
 RotateMode := 0;
 if (X < Groupbox1.Left *0.15) or (X > Groupbox1.Left *0.85) or
   (Y < Groupbox2.Top *0.15) or (Y > Groupbox2.Top *0.85) then begin
      RotateMode := 1;
      StartWinkel := BerechneClickWinkel(X, Y);
   end;

end;

function TParticleEditForm.BerechneClickWinkel(X, Y: Single): Single;
var
   dx, dy: Single;
begin
      dx := X - Groupbox1.Left / 2;
      dy := y - Groupbox2.Top / 2;
      if dx = 0 then
        dx := 0.01;
      result := arctan(dy / dx);
      if (dx < 0) and (dy > 0) then
         result := result + PI;
      if (dx < 0) and (dy < 0) then
         result := result - PI;
end;

procedure TParticleEditForm.FormMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
 FButton := False;
 MouseCapture := False;

end;

procedure TParticleEditForm.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
  Winkel: Single;
  B: array[0..3] of Boolean;

begin
 B[0] := X < Groupbox1.Left *0.15;
 B[1] := X > Groupbox1.Left *0.85;
 B[2] := Y < Groupbox2.Top * 0.15;
 B[3] := Y > Groupbox2.Top * 0.85;
 if B[0] or B[1] or B[2] or B[3] then begin
    if (B[0] or B[1]) and not (B[2] or B[3]) then Cursor := crSizeNS;
    if (B[2] or B[3]) and not (B[0] or B[1]) then Cursor := crSizeWE;
    if (B[0] and B[2]) or (B[1] and B[3]) then Cursor := crSizeNESW;
    if (B[0] and B[3]) or (B[1] and B[2]) then Cursor := crSizeNWSE;
   end else
       Cursor := crSizeAll;

  if (FButton) then begin
    if RotateMode = 0 then begin
       glLoadIdentity;
       glRotatef((X - OldX), 0,1,0);
       glMultMatrixf(@ModelView[0,0]);
       glGetFloatv(GL_MODELVIEW_MATRIX, @ModelView[0,0]);

       glLoadIdentity;
       glRotatef((Y - OldY), 1,0,0);
       glMultMatrixf(@ModelView[0,0]);
       glGetFloatv(GL_MODELVIEW_MATRIX, @ModelView[0,0]);
    end else begin
        Winkel := BerechneClickWinkel(X, Y);
        if (Winkel <> StartWinkel) then begin
           glLoadIdentity;
           glRotatef((StartWinkel - Winkel) / PI * 180, 0,0,1);
           glMultMatrixf(@ModelView[0,0]);
           glGetFloatv(GL_MODELVIEW_MATRIX, @ModelView[0,0]);
           StartWinkel := Winkel;
        end;
    end;
  end;
  OldX := X;
  OldY := Y;
end;

procedure TParticleEditForm.EditTypeDefinitions1Click(Sender: TObject);
var
  Form: TForm;
begin
  Form := TParticleTextureForm.Create(Self, FEffectDefinition, FParticleHandler);
  Form.ShowModal;
  Update_TypeList;
end;

procedure TParticleEditForm.SaveDefinitionFile1Click(Sender: TObject);
begin
 if SaveDialog.Execute then begin
     FEffectDefinition.SaveToFile (SaveDialog.FileName);
 end;
end;

procedure TParticleEditForm.Update_ParticleList;
var
  Index: Integer;
  Item: TListItem;
  Particle: TParticleDefinition;
begin
  ListView2.Items.BeginUpdate;
  ListView2.Items.Clear;
  For Index := 0 to FEffectDefinition.ParticleCount - 1 do begin
      Particle := FEffectDefinition.Particles[Index];
      Item := ListView2.Items.Add;
      Item.Caption := IntToStr(Particle.ID);
      Item.SubItems.Add (Particle.Description);
      Item.SubItems.Add (Format ('%.0f +/- %.0f ms', [ Particle.AppearTime.Average, Particle.AppearTime.Variance ]));
      Item.SubItems.Add (IntToStr(Particle.Amount));
      Item.Data := Particle;
  end;
  ListView2.Items.EndUpdate;
end;

procedure TParticleEditForm.SetEnable;
var
  Index: Integer;
begin
  For Index := 0 to Groupbox1.ControlCount - 1 do
    Groupbox1.Controls[Index].Enabled := Assigned(ListView2.Selected);
  Button3.Enabled := Assigned(ListView2.Selected);
  Button9.Enabled := Assigned(ListView2.Selected);

  if Assigned(ListView2.Selected) then
      ListView1.Color := clWindow
  else
      ListView1.Color := clBtnFace;

  Button5.Enabled := Assigned(ListView1.Selected);
  Button6.Enabled := Assigned(ListView1.Selected);
  Button7.Enabled := Assigned(ListView1.Selected);
end;

procedure TParticleEditForm.Button2Click(Sender: TObject);
var
  Particle: TParticleDefinition;
  Index: Integer;
begin
  Particle := FEffectDefinition.AddParticle;
  Update_ParticleList;
  For Index := 0 to ListView2.Items.Count - 1 do
      if ListView2.Items[Index].Data = Particle then
          ListView2.Selected := ListView2.Items[Index];
end;

procedure TParticleEditForm.ShowEffect;
begin
      Edit16.Text := FEffectDefinition.Description;
      Edit18.Text := FEffectDefinition.Name;
      Radiobutton1.Checked := FEffectDefinition.AppearType = atPeriodical;
      Radiobutton2.Checked := FEffectDefinition.AppearType = atOnce;
      Edit17.Text := IntToStr(FEffectDefinition.Time);
      Edit19.Text := IntToStr(FEffectDefinition.EffectID);
end;

procedure TParticleEditForm.ShowEmissions;
var
   ParticleDefinition: TParticleDefinition;
   Index: Integer;
   ListItem: TListItem;
begin
   ListView1.Items.Clear;
   if Assigned (ListView2.Selected) then begin
      ParticleDefinition := ListView2.Selected.Data;
         ListView1.Items.BeginUpdate;
         For Index := 0 to ParticleDefinition.EmissionCount - 1 do begin
            ListItem := ListView1.Items.Add;
            ListItem.Caption := ParticleDefinition.Emissions[Index].Name;
            ListItem.SubItems.Add(ParticleDefinition.Emissions[Index].Parameter);
            ListItem.Data := ParticleDefinition.Emissions[Index];
         end;
         ListView1.Items.EndUpdate;
   end;
end;

procedure TParticleEditForm.Rotation1Click(Sender: TObject);
var
  Emission: TEmissionDefinition;
  ParticleDefinition: TParticleDefinition;
begin
   if Assigned (ListView2.Selected) then begin
      ParticleDefinition := ListView2.Selected.Data;
      Emission := nil;
      if Sender = Rotation1 then
          Emission := TEmission_Rotation.Create;
      if Sender = Circle1 then
          Emission := TEmission_Circle.Create;
      if Sender = Sphere1 then
          Emission := TEmission_Sphere.Create;
      if Sender = Translation1 then
          Emission := TEmission_Translation.Create;

      Assert (Assigned (Emission));
      ParticleDefinition.AddEmission(Emission);
      ShowEmissions;
   end;
end;


procedure TParticleEditForm.Button1Click(Sender: TObject);
var
  P: TPoint;
begin
  P := Button1.ClientToScreen(Point(30,15));
  PopupMenu1.Popup(P.X, P.Y);
end;

procedure TParticleEditForm.Button4Click(Sender: TObject);
begin
   FParticleHandler.ClearParticles;
   FParticleHandler.Start;
end;

procedure TParticleEditForm.Button5Click(Sender: TObject);
var
  ParticleDefinition: TParticleDefinition;
begin
   if Assigned (ListView2.Selected) and Assigned (ListView1.Selected) then begin
      ParticleDefinition := ListView2.Selected.Data;
      ParticleDefinition.RemoveEmission (ListView1.Selected.Data);
      ShowEmissions;
   end;
end;

procedure TParticleEditForm.ListView1DblClick(Sender: TObject);
var
  Emission: TEmissionDefinition;
  RotationForm: TRotationEditForm;
  TranslationForm: TTranslationEditForm;
  S: String;
begin
   if Assigned (ListView1.Selected) then begin
      Emission := ListView1.Selected.Data;
      if Emission is TEmission_Rotation then begin
          RotationForm := TRotationEditForm.Create (Self);
          try
              RotationForm.Edit1.Text := Format ('%.1f', [TEmission_Rotation(Emission).Angle1]);
              RotationForm.Edit2.Text := Format ('%.1f', [TEmission_Rotation(Emission).Angle2]);
              if RotationForm.ShowModal = mrOK then begin
                  TEmission_Rotation(Emission).Angle1 := StrToFloat (RotationForm.Edit1.Text);
                  TEmission_Rotation(Emission).Angle2 := StrToFloat (RotationForm.Edit2.Text);
              end;
          finally
              RotationForm.Free;
          end;
      end;

      if Emission is TEmission_Translation then begin
          TranslationForm := TTranslationEditForm.Create (Self);
          try
              TranslationForm.Edit1.Text := Format ('%.3f', [TEmission_Translation(Emission).X.Average]);
              TranslationForm.Edit2.Text := Format ('%.3f', [TEmission_Translation(Emission).X.Variance]);
              TranslationForm.Edit3.Text := Format ('%.3f', [TEmission_Translation(Emission).Y.Average]);
              TranslationForm.Edit4.Text := Format ('%.3f', [TEmission_Translation(Emission).Y.Variance]);
              TranslationForm.Edit5.Text := Format ('%.3f', [TEmission_Translation(Emission).Z.Average]);
              TranslationForm.Edit6.Text := Format ('%.3f', [TEmission_Translation(Emission).Z.Variance]);
              if TranslationForm.ShowModal = mrOK then begin
                  TEmission_Translation(Emission).X.Average := StrToFloat (TranslationForm.Edit1.Text);
                  TEmission_Translation(Emission).X.Variance := StrToFloat (TranslationForm.Edit2.Text);
                  TEmission_Translation(Emission).Y.Average := StrToFloat (TranslationForm.Edit3.Text);
                  TEmission_Translation(Emission).Y.Variance := StrToFloat (TranslationForm.Edit4.Text);
                  TEmission_Translation(Emission).Z.Average := StrToFloat (TranslationForm.Edit5.Text);
                  TEmission_Translation(Emission).Z.Variance := StrToFloat (TranslationForm.Edit6.Text);
              end;
          finally
              TranslationForm.Free;
          end;
      end;

      if Emission is TEmission_Circle then begin
          S := InputBox ('Edit Circle', 'Radius: ', Format ('%.3f', [TEmission_Circle(Emission).Radius]));
          TEmission_Circle(Emission).Radius := StrToFloat (S);
      end;

      if Emission is TEmission_Sphere then begin
          S := InputBox ('Edit Sphere', 'Radius: ', Format ('%.3f', [TEmission_Sphere(Emission).Radius]));
          TEmission_Sphere(Emission).Radius := StrToFloat (S);
      end;

      ListView1.Selected.SubItems[0] := Emission.Parameter;
   end;
end;

procedure TParticleEditForm.Button6Click(Sender: TObject);
var
  ParticleDefinition: TParticleDefinition;
  Emission: TEmissionDefinition;
  Index: Integer;
begin
   if Assigned (ListView2.Selected) and Assigned (ListView1.Selected) then begin
      ParticleDefinition := ListView2.Selected.Data;
      Emission := ListView1.Selected.Data;

      if Sender = Button6 then // Move Up
          ParticleDefinition.MoveEmission (Emission, True);
      if Sender = Button7 then // Move Down
          ParticleDefinition.MoveEmission (Emission, False);
      ShowEmissions;
      For Index := 0 to ListView1.Items.Count - 1 do
          if ListView1.Items[Index].Data = Emission then
              ListView1.Selected := ListView1.Items[Index];
   end;
end;

procedure TParticleEditForm.ListView1Change(Sender: TObject; Item: TListItem;
  Change: TItemChange);
begin
  Button5.Enabled := Assigned(ListView1.Selected);
  Button6.Enabled := Assigned(ListView1.Selected);
  Button7.Enabled := Assigned(ListView1.Selected);
end;

procedure TParticleEditForm.Button3Click(Sender: TObject);
begin
   if Assigned (ListView2.Selected) then begin
      FEffectDefinition.RemoveParticle(TParticleDefinition(ListView2.Selected.Data).ID);
      ListView2.DeleteSelected;
      SetEnable;
   end;
end;

procedure TParticleEditForm.FormMouseWheel(Sender: TObject;
  Shift: TShiftState; WheelDelta: Integer; MousePos: TPoint;
  var Handled: Boolean);
var
 NewZoom: Single;
begin
  if WheelDelta > 0 then
      NewZoom := FZoom / 0.9
  else
      NewZoom := FZoom * 0.9;
  if (NewZoom > 0.6) and (NewZoom < 20) then FZoom := NewZoom;
end;

procedure TParticleEditForm.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
  GLEngine.Free;
  GLEngine := nil;
  Inititalised := False;
  Action := caFree;
  FEffectDefinition.UnLock;
end;

procedure TParticleEditForm.SetBackgroundColor1Click(Sender: TObject);
begin
  ColorDialog1.Color := Setting_BackgroundColor;
  if ColorDialog1.Execute then begin
      Setting_BackgroundColor := ColorDialog1.Color;
      SaveSettings;

      GLEngine.MakeCurrent(Canvas.Handle);
      GLEngine.SetClearColor (Setting_BackgroundColor);
  end;

end;

procedure TParticleEditForm.Button9Click(Sender: TObject);
var
  Particle: TParticleDefinition;
  Index: Integer;
begin
  if Assigned (ListView2.Selected) then begin
      Particle := FEffectDefinition.AddParticle;
      Particle.Apply (ListView2.Selected.Data);
      Update_ParticleList;
      For Index := 0 to ListView2.Items.Count - 1 do
          if ListView2.Items[Index].Data = Particle then
              ListView2.Selected := ListView2.Items[Index];
  end;
end;

procedure TParticleEditForm.FormActivate(Sender: TObject);
begin
     Update_TypeList;

end;

procedure TParticleEditForm.FormShow(Sender: TObject);
begin
     Update_TypeList;
     Update_ParticleList;
     ShowEffect;
     SetEnable;

end;

end.
