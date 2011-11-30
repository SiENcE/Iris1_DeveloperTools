unit form_Settings;
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
    Label1: TLabel;
    Label5: TLabel;
    ComboBox1: TComboBox;
    ComboBox2: TComboBox;
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
   case Combobox1.ItemIndex of
      0: ColorDepth := 16;
      1: ColorDepth := 24;
      2: ColorDepth := 32;
   end;
   case Combobox2.ItemIndex of
      0: ZBufferDepth := 16;
      1: ZBufferDepth := 24;
      2: ZBufferDepth := 32;
   end;
   SaveSettings;
end;

procedure TSettingsForm.FormShow(Sender: TObject);
begin
   Edit2.Text := TileDataMul;
   Edit3.Text := ArtMul;
   Edit4.Text := ArtIdxMul;
   Case ColorDepth of
      16: Combobox1.ItemIndex := 0;
      24: Combobox1.ItemIndex := 1;
      else Combobox1.ItemIndex := 2;
   end;
   Case ZBufferDepth of
      16: Combobox2.ItemIndex := 0;
      24: Combobox2.ItemIndex := 1;
      else Combobox2.ItemIndex := 2;
   end;
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
