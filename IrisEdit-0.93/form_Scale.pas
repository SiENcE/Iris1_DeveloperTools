unit form_Scale;
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
