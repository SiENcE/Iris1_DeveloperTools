unit form_Coords;
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
  StdCtrls, ExtCtrls;

type
  TCoordCallback = procedure (AX, AY, AZ: Single) of Object;
  TCoordForm = class(TForm)
    Edit1: TEdit;
    Edit2: TEdit;
    Edit3: TEdit;
    Button1: TButton;
    Button2: TButton;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormDeactivate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Edit1KeyPress(Sender: TObject; var Key: Char);
  private
    { Private-Deklarationen }
    FCallback: TCoordCallback;
  public
    { Public-Deklarationen }
    constructor Create (AOwner: TComponent; AX, AY: Integer; ACallback: TCoordCallback); reintroduce;
  end;


implementation

{$R *.DFM}

constructor TCoordForm.Create (AOwner: TComponent; AX, AY: Integer; ACallback: TCoordCallback);
begin
  inherited Create (AOwner);
  Left := AX - Width;
  Top := AY;
  FCallback := ACallback;
end;


procedure TCoordForm.FormClose(Sender: TObject; var Action: TCloseAction);
begin
   Action := caFree;
end;

procedure TCoordForm.FormDeactivate(Sender: TObject);
begin
   Close;
end;

procedure TCoordForm.Button1Click(Sender: TObject);
begin
   if Assigned(FCallback) then
      FCallback (StrToFloat (Edit1.Text),StrToFloat (Edit2.Text),StrToFloat (Edit3.Text));
   Close;
end;

procedure TCoordForm.Edit1KeyPress(Sender: TObject; var Key: Char);
begin
  if Key = #13 then begin
     Key := #0;
     Button1Click (Sender);
  end;
  if Key = #27 then begin
     Key := #0;
     FormDeactivate (Sender);
  end;
end;

end.
