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

unit form_Add3DS;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtDlgs, Unit_Models;

type
  TImport3DSForm = class(TForm)
    Label1: TLabel;
    Edit1: TEdit;
    Button1: TButton;
    Label2: TLabel;
    Edit2: TEdit;
    Button2: TButton;
    CheckBox1: TCheckBox;
    OpenDialog3DS: TOpenDialog;
    OpenPictureDialog: TOpenPictureDialog;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    RadioButton3: TRadioButton;
    Edit3: TEdit;
    Button3: TButton;
    Button4: TButton;
    procedure Button2Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private-Deklarationen }
    FFileName: String;
    FTextureFileName: String;
    FScaleFactor: Single;
    FScaleMode: T3DSScaleMode;
    FTranslateToDefaultPos: Boolean;
  public
    { Public-Deklarationen }
    property FileName: String read FFileName;
    property TextureFileName: String read FTextureFileName;
    property ScaleFactor: Single read FScaleFactor;
    property ScaleMode: T3DSScaleMode read FScaleMode;
    property TranslateToDefaultPos: Boolean read FTranslateToDefaultPos;
  end;

implementation

{$R *.dfm}

procedure TImport3DSForm.Button1Click(Sender: TObject);
begin
   if OpenDialog3DS.Execute then begin
      Edit1.Text := OpenDialog3DS.FileName;
      Button3.Enabled := True;
   end;
end;

procedure TImport3DSForm.Button3Click(Sender: TObject);
begin
    FFileName := Edit1.Text;
    FTextureFileName := Edit2.Text;
    FScaleMode := smNone;
    FScaleFactor := 1;
    if RadioButton2.Checked then
        FScaleMode := smAuto;
    if RadioButton3.Checked then begin
        FScaleMode := smCustom;
        FScaleFactor := 20 / StrToFloat (Edit3.Text);
    end;
    FTranslateToDefaultPos := Checkbox1.Checked;
   ModalResult := mrOK;
end;

procedure TImport3DSForm.Button4Click(Sender: TObject);
begin
   ModalResult := mrCancel;
end;

procedure TImport3DSForm.Button2Click(Sender: TObject);
begin
   if OpenPictureDialog.Execute then
      Edit2.Text := OpenPicturedialog.FileName;
end;

end.
