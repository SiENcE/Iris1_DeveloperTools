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

unit form_Progress;

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
