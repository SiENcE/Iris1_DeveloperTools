unit Form_Textures;
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
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, pngimage, ComCtrls, Unit_Particles, Unit_Settings;

type
  TParticleTextureForm = class(TForm)
    GroupBox1: TGroupBox;
    Image1: TImage;
    Button1: TButton;
    ListView1: TListView;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    GroupBox2: TGroupBox;
    Label5: TLabel;
    Edit5: TEdit;
    Label2: TLabel;
    Edit1: TEdit;
    Label1: TLabel;
    Edit2: TEdit;
    Label3: TLabel;
    Edit3: TEdit;
    Label4: TLabel;
    Edit4: TEdit;
    OpenDialog: TOpenDialog;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure ListView1Change(Sender: TObject; Item: TListItem;
      Change: TItemChange);
    procedure Edit1KeyPress(Sender: TObject; var Key: Char);
    procedure Edit1Change(Sender: TObject);
    procedure SetEnable;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button4Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private-Deklarationen }
    FEffectDefinition: TEffectDefinition;
    FParticleHandler: TParticleHandler;
  public
    { Public-Deklarationen }
    constructor Create (AOwner: TComponent; AEffectDefinition: TEffectDefinition; AParticleHandler: TParticleHandler); reintroduce;
  end;

implementation

{$R *.DFM}

constructor TParticleTextureForm.Create (AOwner: TComponent; AEffectDefinition: TEffectDefinition; AParticleHandler: TParticleHandler);
begin
    inherited Create (AOwner);
    FEffectDefinition := AEffectDefinition;
    FParticleHandler := AParticleHandler;
end;

procedure TParticleTextureForm.Button1Click(Sender: TObject);
var
  PNG: TPNGObject;
begin
  {Creates and loads the input images}
  if OpenDialog.Execute then begin
    PNG := TPNGObject.Create;
    try
        PNG.LoadFromFile(OpenDialog.FileName);
        Image1.Canvas.FillRect(Image1.Canvas.ClipRect);
        Image1.Canvas.Draw(0, 0, PNG);
        FParticleHandler.LoadTexture(PNG);
        FEffectDefinition.TextureFileName := ExtractFileName(OpenDialog.FileName);
    finally
        PNG.Free;
    end;
  end;
end;


procedure TParticleTextureForm.Button2Click(Sender: TObject);
begin
  Close;
end;

procedure TParticleTextureForm.FormActivate(Sender: TObject);
var
   Index: Integer;
   Item: TListItem;
   ParticleType: TParticleTypeDefinition;
begin
     ListView1.Items.Clear;
     For Index := 0 to FEffectDefinition.TypeCount - 1 do begin
         ParticleType := FEffectDefinition.Types[Index];
         Item := ListView1.Items.Add;
         Item.Caption := IntToStr(ParticleType.ID);
         Item.SubItems.Add(ParticleType.Description);
         Item.SubItems.Add(Format('%d,%d', [ParticleType.TexX, ParticleType.TexY]));
         Item.SubItems.Add(Format('%dx%d', [ParticleType.Width, ParticleType.Height]));
         Item.Data := Pointer(ParticleType.ID);
     end;
     SetEnable;
end;

procedure TParticleTextureForm.Button3Click(Sender: TObject);
var
   ParticleType: TParticleTypeDefinition;
begin
     ParticleType := FEffectDefinition.AddType;
     ParticleType.Width := 256;
     ParticleType.Height := 256;
     ParticleType.Description := 'New Type';
     FormActivate(Sender);
     ListView1.Selected := ListView1.Items[ListView1.Items.Count - 1];
end;

procedure TParticleTextureForm.ListView1Change(Sender: TObject; Item: TListItem;
  Change: TItemChange);
var
   ParticleType: TParticleTypeDefinition;
begin
     if Assigned(ListView1.Selected) then begin
         ParticleType := FEffectDefinition.GetType(Cardinal(ListView1.Selected.Data));
         with ParticleType do begin
            Edit1.Text := IntToStr(TexX);
            Edit2.Text := IntToStr(TexY);
            Edit3.Text := IntToStr(Width);
            Edit4.Text := IntToStr(Height);
            Edit5.Text := Description;
         end;
     end;
     SetEnable;
end;

procedure TParticleTextureForm.Edit1KeyPress(Sender: TObject; var Key: Char);
begin
  if not (key in ['0'..'9', #8]) then key := #0;
end;

procedure TParticleTextureForm.Edit1Change(Sender: TObject);
var
   ParticleType: TParticleTypeDefinition;
begin
     if Assigned(ListView1.Selected) then begin
         ParticleType := FEffectDefinition.GetType(Cardinal(ListView1.Selected.Data));
         with ParticleType do begin
              if Sender = Edit1 then TexX := StrToIntDef(Edit1.Text, 0);
              if Sender = Edit2 then TexY := StrToIntDef(Edit2.Text, 0);
              if Sender = Edit3 then Width := StrToIntDef(Edit3.Text, 0);
              if Sender = Edit4 then Height := StrToIntDef(Edit4.Text, 0);
              if Sender = Edit5 then Description := Edit5.Text;
         end;
         with ListView1.Selected do begin
            SubItems[0] := ParticleType.Description;
            SubItems[1] := (Format('%d,%d', [ParticleType.TexX, ParticleType.TexY]));
            SubItems[2] := (Format('%dx%d', [ParticleType.Width, ParticleType.Height]));
         end;
     end;
end;

procedure TParticleTextureForm.SetEnable;
var
  Index: Integer;
begin
  For Index := 0 to Groupbox2.ControlCount - 1 do
    Groupbox2.Controls[Index].Enabled := Assigned (ListView1.Selected);
  Button4.Enabled := Assigned (ListView1.Selected);
end;

procedure TParticleTextureForm.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
  Action := caFree;
end;

procedure TParticleTextureForm.Button4Click(Sender: TObject);
begin
     if Assigned(ListView1.Selected) then begin
        FEffectDefinition.RemoveType (Cardinal(ListView1.Selected.Data));
        FormActivate (Sender);
     end;
end;

procedure TParticleTextureForm.FormShow(Sender: TObject);
var
  PNG: TPNGObject;
begin
  if FileExists (Setting_TextureDir + FEffectDefinition.TextureFileName) then begin
    PNG := TPNGObject.Create;
    try
        PNG.LoadFromFile(Setting_TextureDir + FEffectDefinition.TextureFileName);
        Image1.Canvas.FillRect(Image1.Canvas.ClipRect);
        Image1.Canvas.Draw(0, 0, PNG);
    finally
        PNG.Free;
    end;
  end;
end;

end.
