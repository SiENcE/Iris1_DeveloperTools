unit form_Coords;
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
