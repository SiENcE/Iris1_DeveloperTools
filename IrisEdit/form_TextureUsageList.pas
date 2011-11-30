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

unit form_TextureUsageList;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, ComCtrls, Unit_TextureManager, Unit_ModelLoader,
  Unit_Models, Unit_TileDataLoader;

type
  TTextureUsageForm = class(TForm)
    ListView: TListView;
    Button1: TButton;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Edit1: TEdit;
    Label2: TLabel;
    Edit2: TEdit;
    Label4: TLabel;
    Edit4: TEdit;
    Edit3: TEdit;
    Label3: TLabel;
    PaintBox: TPaintBox;
    Button2: TButton;
    procedure ListViewChange(Sender: TObject; Item: TListItem;
      Change: TItemChange);
    procedure Button1Click(Sender: TObject);
    procedure PaintBoxPaint(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private-Deklarationen }
    FCurrentBitmap: TBitmap;
    FResultModelID: Integer;
  public
    { Public-Deklarationen }
    constructor Create (AOwner: TComponent; ATexture: TJPEGTexture); reintroduce;
    destructor Destroy; override;
    property ResultModelID: Integer read FResultModelID;
  end;


implementation

{$R *.dfm}

constructor TTextureUsageForm.Create (AOwner: TComponent; ATexture: TJPEGTexture);
var
  Item: TListItem;
  Index: Integer;
  ACount: Integer;
  AModel: TModel;
  S: String;
begin
   inherited Create (AOwner);
   Assert (Assigned (ATexture));
   Button1.Enabled := False;
   Edit1.Text := ATexture.Name;
   Edit2.Text := Format('%dx%d', [ATexture.Width, ATexture.Height]);
   Edit3.Text := ATexture.MD5Sum;
   Edit4.Text := IntToStr (ATexture.UseCount);
   Paintbox.ControlStyle := Paintbox.ControlStyle + [csOpaque];
   FCurrentBitmap := ATexture.CreateBitmap;

   ListView.Items.BeginUpdate;
   For Index := 0 to 65535 do begin
       AModel := pModelLoader.Get (Index);
       if Assigned (AModel) then begin
          ACount := AModel.UsesTexture (ATexture);
          if ACount > 0 then begin
              Item := ListView.Items.Add;
              Item.Caption := IntToStr (AModel.ModelID);
              S := '';
              if Index < pTileDataLoader.Count then
                  S := pTileDataLoader.Items[Index].Name;
              if S = '' then S := '<unnamed>';
              Item.SubItems.Add (S);
              Item.SubItems.Add (IntToStr (AModel.NodeCount));
              Item.SubItems.Add (IntToStr (AModel.FaceCount));
              Item.SubItems.Add (AModel.GetFlagString);
              Item.SubItems.Add (IntToStr (ACount) + 'x');
              Item.Data := AModel;
          end;
       end;
   end;
   ListView.Items.EndUpdate;
end;

destructor TTextureUsageForm.Destroy;
begin
    FCurrentBitmap.Free;
    inherited Destroy;
end;

procedure TTextureUsageForm.FormClose(Sender: TObject; var Action: TCloseAction);
begin
   Action := caFree;
end;

procedure TTextureUsageForm.Button2Click(Sender: TObject);
begin
    Close;
end;

procedure TTextureUsageForm.PaintBoxPaint(Sender: TObject);
begin
  Paintbox.Canvas.Brush.Color := clGray;
  Paintbox.Canvas.FillRect (Paintbox.Canvas.ClipRect);
  if Assigned (FCurrentBitmap) then begin
     Paintbox.Canvas.Draw ((PaintBox.Width - FCurrentBitmap.Width) div 2,
                           (PaintBox.Height - FCurrentBitmap.Height) div 2,
                           FCurrentBitmap);
  end;
end;

procedure TTextureUsageForm.Button1Click(Sender: TObject);
var
    AModel: TModel;
begin
    if Assigned (ListView.Selected) then begin
        AModel := ListView.Selected.Data;
        if Assigned (AModel) then
            FResultModelID := AModel.ModelID;
        ModalResult := mrOK;
    end;
end;

procedure TTextureUsageForm.ListViewChange(Sender: TObject; Item: TListItem;
  Change: TItemChange);
begin
   Button1.Enabled := Assigned (ListView.Selected);
end;

end.
