unit form_Main;
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
  Dialogs, Menus, ComCtrls, Unit_Particles, Unit_Settings, form_ParticleEdit, FileCtrl;

const
  VersionString = '0.93';

type
  TMainForm = class(TForm)
    MainMenu1: TMainMenu;
    Particles1: TMenuItem;
    LoadCollection: TMenuItem;
    AddCollection1: TMenuItem;
    SaveCollectionas1: TMenuItem;
    N1: TMenuItem;
    Exit1: TMenuItem;
    extures1: TMenuItem;
    SetTextureDir1: TMenuItem;
    Effects1: TMenuItem;
    AddEffect1: TMenuItem;
    ListView1: TListView;
    LoadEffect1: TMenuItem;
    SaveDialog: TSaveDialog;
    OpenDialog: TOpenDialog;
    OpenEffectDialog: TOpenDialog;
    PopupMenu1: TPopupMenu;
    Add2: TMenuItem;
    Remove1: TMenuItem;
    Load1: TMenuItem;
    procedure Exit1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure AddEffect1Click(Sender: TObject);
    procedure SaveCollectionas1Click(Sender: TObject);
    procedure LoadCollectionClick(Sender: TObject);
    procedure LoadEffect1Click(Sender: TObject);
    procedure ListView1DblClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure Remove1Click(Sender: TObject);
    procedure SetTextureDir1Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private-Deklarationen }
    FCollection: TEffectCollection;

    procedure ShowEffects;
  public
    { Public-Deklarationen }
  end;

var
  MainForm: TMainForm;

implementation

{$R *.dfm}

procedure TMainForm.Exit1Click(Sender: TObject);
begin
  Close;
end;

procedure TMainForm.FormCreate(Sender: TObject);
begin
    Application.UpdateFormatSettings := False;

    Caption := Caption + ' ' + VersionString;

    ThousandSeparator := #0;
    DecimalSeparator := '.';

    FCollection := TEffectCollection.Create;
    LoadSettings;
    ShowEffects;
    if Setting_TextureDir = '' then
        Setting_TextureDir := ExtractFilePath (Application.ExeName);

end;

procedure TMainForm.FormDestroy(Sender: TObject);
begin
    FCollection.Free;
    FCollection := nil;

end;

procedure TMainForm.ShowEffects;
var
  Index: Integer;
  Item: TListItem;
  Effect: TEffectDefinition;
begin
  ListView1.Items.BeginUpdate;
  ListView1.Items.Clear;
  For Index := 0 to FCollection.Count - 1 do begin
      Effect := FCollection.Items[Index];
      Item := ListView1.Items.Add;
      Item.Data := Effect;
      Item.Caption := Effect.Name;
      Item.SubItems.Add(Effect.Description);
      Item.SubItems.Add(Effect.TextureFileName);
      Item.SubItems.Add(Effect.AppearString);
  end;
  ListView1.Items.EndUpdate;
end;

procedure TMainForm.AddEffect1Click(Sender: TObject);
var
  Effect: TEffectDefinition;
  AName: String;
  Form: TParticleEditForm;
begin
  AName := Inputbox ('Create Effect', 'Name: ', '');
  if AName <> '' then begin
      Effect := TEffectDefinition.Create;
      Effect.Name := AName;
      FCollection.AddEffect(Effect);
      ShowEffects;
      Form := TParticleEditForm.Create (Self, Effect);
      Form.Show;

  end;
end;

procedure TMainForm.SaveCollectionas1Click(Sender: TObject);
begin
   if SaveDialog.Execute then
      FCollection.SaveToFile(SaveDialog.FileName);
end;

procedure TMainForm.LoadCollectionClick(Sender: TObject);
var
  Index: Integer;
  ACollection: TEffectCollection;
  AStream: TStream;
begin
   For Index := 0 to FCollection.Count - 1 do
      if FCollection.Items[Index].LockFlag then begin
          MessageDlg ('Please close all windows before loading a collection...', mtError, [mbOK], 0);
          exit;
      end;

   if OpenDialog.Execute then begin
      AStream := TFileStream.Create (OpenDialog.FileName, fmOpenRead);
      try
        ACollection := TEffectCollection.CreateFromStream(AStream, AStream.Size);
      finally
          AStream.Free;
      end;

      FCollection.Free;
      FCollection := ACollection;
   end;

   ShowEffects;
end;

procedure TMainForm.LoadEffect1Click(Sender: TObject);
var
  Effect: TEffectDefinition;
  Form: TParticleEditForm;
  AStream: TStream;
begin
  if OpenEffectDialog.Execute then begin
      AStream := TFileStream.Create (OpenEffectDialog.FileName, fmOpenRead);
      try
        Effect := TEffectDefinition.CreateFromStream (AStream, AStream.Size);
      finally
          AStream.Free;
      end;
      FCollection.AddEffect(Effect);
      ShowEffects;
      Form := TParticleEditForm.Create (Self, Effect);
      Form.Show;
  end;
end;

procedure TMainForm.ListView1DblClick(Sender: TObject);
var
  Form: TParticleEditForm;
begin
  if Assigned (ListView1.Selected) then begin
      Form := TParticleEditForm.Create (Self, ListView1.Selected.Data);
      Form.Show;
  end;
end;

procedure TMainForm.FormActivate(Sender: TObject);
begin
   ShowEffects;
end;

procedure TMainForm.Remove1Click(Sender: TObject);
var
   Effect: TEffectDefinition;
begin
  if Assigned (ListView1.Selected) then
    if MessageDlg ('Are you sure?', mtConfirmation, [mbYes, mbNo], 0) = mrYes then begin
     Effect := ListView1.Selected.Data;
     Effect.Lock;
     FCollection.RemoveEffect(Effect);
     ShowEffects;
  end;

end;

procedure TMainForm.SetTextureDir1Click(Sender: TObject);
var
  TextureDir: String;
begin
     if SelectDirectory ('Please choose a texture directory', '', TextureDir) then begin
        Setting_TextureDir := IncludeTrailingBackslash(TextureDir);
        SaveSettings;
     end;

end;

procedure TMainForm.FormShow(Sender: TObject);
var
  ACollection: TEffectCollection;
  AStream: TStream;
  FileName: String;
begin
   FileName := ExtractFilePath(Application.ExeName) + 'ParticleEdit.xml';
   try
     if FileExists (FileName) then begin
      AStream := TFileStream.Create (FileName, fmOpenRead);
      try
        ACollection := TEffectCollection.CreateFromStream(AStream, AStream.Size);
      finally
          AStream.Free;
      end;

      FCollection.Free;
      FCollection := ACollection;
     end;
   except
   end;

   ShowEffects;
end;

procedure TMainForm.FormClose(Sender: TObject; var Action: TCloseAction);
var
  FileName: String;
begin
   FileName := ExtractFilePath(Application.ExeName) + 'ParticleEdit.xml';
   FCollection.SaveToFile(FileName);
end;

end.
