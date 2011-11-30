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

unit form_Settings;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Unit_Settings, FileCtrl;

type
  TSettingsForm = class(TForm)
    Label2: TLabel;
    Edit2: TEdit;
    Button2: TButton;
    Label3: TLabel;
    Edit3: TEdit;
    Button3: TButton;
    Button4: TButton;
    Edit4: TEdit;
    Label4: TLabel;
    Button5: TButton;
    Button6: TButton;
    OpenDialog: TOpenDialog;
    procedure Button5Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Button3Click(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  SettingsForm: TSettingsForm;

implementation

{$R *.DFM}

procedure TSettingsForm.Button5Click(Sender: TObject);
begin
   MessageDlg ('You have to restart the editor in order to apply all the changes', mtInformation, [ mbOK ], 0);
   TileDataMul := Edit2.Text;
   ArtMul := Edit3.Text;
   ArtIdxMul := Edit4.Text;
   SaveSettings;
end;

procedure TSettingsForm.FormShow(Sender: TObject);
begin
   Edit2.Text := TileDataMul;
   Edit3.Text := ArtMul;
   Edit4.Text := ArtIdxMul;
end;

procedure TSettingsForm.Button3Click(Sender: TObject);
begin
  if Sender = Button2 then begin
      OpenDialog.FileName := Edit2.Text;
      OpenDialog.Filter := 'UO TileData Files|tiledata.mul';
  end;
  if Sender = Button3 then begin
      OpenDialog.FileName := Edit3.Text;
      OpenDialog.Filter := 'UO Art Files|art.mul';
  end;
  if Sender = Button4 then begin
      OpenDialog.FileName := Edit4.Text;
      OpenDialog.Filter := 'UO ArtIdx Files|artidx.mul';
  end;

  if OpenDialog.Execute then begin
     if Sender = Button2 then
         Edit2.Text := OpenDialog.FileName;
     if Sender = Button3 then
         Edit3.Text := OpenDialog.FileName;
     if Sender = Button4 then
         Edit4.Text := OpenDialog.FileName;
  end;
end;

end.
