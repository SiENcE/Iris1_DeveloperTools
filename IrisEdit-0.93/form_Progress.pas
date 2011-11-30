unit form_Progress;
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
  Dialogs, ComCtrls, ExtCtrls, StdCtrls;

type
  TProgressForm = class(TForm)
    Panel1: TPanel;
    ProgressBar1: TProgressBar;
    InfoText: TLabel;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
    procedure SetStatus (APercentage: Integer; ACaption: String);
  end;

implementation

{$R *.dfm}

procedure TProgressForm.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
   Action := caFree;
end;

procedure TProgressForm.SetStatus (APercentage: Integer; ACaption: String);
begin
  if ACaption <> '' then begin
      InfoText.Caption := ACaption;
      InfoText.Update;
  end;
  ProgressBar1.Position := APercentage;
  ProgressBar1.Update;
end;

end.
