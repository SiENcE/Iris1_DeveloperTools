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

unit form_Info;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, jpeg, ExtCtrls, ShellAPI;

type
  TInfoForm = class(TForm)
    Image1: TImage;
    Memo1: TMemo;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    procedure Button2Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

implementation

{$R *.dfm}

procedure TInfoForm.Button3Click(Sender: TObject);
begin
   Close;
end;

procedure TInfoForm.Button1Click(Sender: TObject);
begin
   ShellExecute (0, 'open', 'http://www.mozilla.org/MPL/MPL-1.1.html', nil, nil, SW_SHOW);
end;

procedure TInfoForm.Button2Click(Sender: TObject);
begin
   ShellExecute (0, 'open', 'http://iris.berlios.de', nil, nil, SW_SHOW);
end;

end.
