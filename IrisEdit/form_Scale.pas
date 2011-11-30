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

unit form_Scale;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TScaleForm = class(TForm)
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Edit1: TEdit;
    Edit2: TEdit;
    Edit3: TEdit;
    Button2: TButton;
    Button3: TButton;
    CheckBox1: TCheckBox;
    procedure CheckBox1Click(Sender: TObject);
    procedure Edit1Change(Sender: TObject);
    procedure Edit1KeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

implementation

{$R *.dfm}

procedure TScaleForm.CheckBox1Click(Sender: TObject);
begin
   Edit2.Enabled := not CheckBox1.Checked;
   Edit3.Enabled := not CheckBox1.Checked;
   if Checkbox1.Checked then begin
      Edit2.Text := Edit1.Text;
      Edit3.Text := Edit1.Text;
   end;
end;

procedure TScaleForm.Edit1Change(Sender: TObject);
begin
   if Checkbox1.Checked then begin
      Edit2.Text := Edit1.Text;
      Edit3.Text := Edit1.Text;
   end;
end;

procedure TScaleForm.Edit1KeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_RETURN then begin
      ModalResult := mrOK;
      Key := 0;
  end;
  if Key = VK_ESCAPE then begin
      ModalResult := mrCancel;
      Key := 0;
  end;
end;

end.
