unit form_TextureManager;
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
  ExtCtrls, ComCtrls, Menus, unit_TextureManager, ExtDlgs, StdCtrls, Unit_IntegerList,
  unit_ArtLoader;

type
  TTextureManagerForm = class(TForm)
    ListView: TListView;
    Panel1: TPanel;
    Splitter1: TSplitter;
    MainMenu1: TMainMenu;
    Texture1: TMenuItem;
    AddTexture1: TMenuItem;
    N1: TMenuItem;
    Close1: TMenuItem;
    StatusBar1: TStatusBar;
    OpenPictureDialog: TOpenPictureDialog;
    PaintBox: TPaintBox;
    ColorDialog: TColorDialog;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Shape1: TShape;
    Label4: TLabel;
    Edit1: TEdit;
    Edit2: TEdit;
    Edit3: TEdit;
    CheckBox1: TCheckBox;
    Button1: TButton;
    Edit4: TEdit;
    GroupBox2: TGroupBox;
    ListBox1: TListBox;
    Edit5: TEdit;
    Button2: TButton;
    Button3: TButton;
    PaintBox1: TPaintBox;
    Button4: TButton;
    SavePictureDialog1: TSavePictureDialog;
    procedure Close1Click(Sender: TObject);
    procedure AddTexture1Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure ListViewChange(Sender: TObject; Item: TListItem;
      Change: TItemChange);
    procedure FormDestroy(Sender: TObject);
    procedure PaintBoxPaint(Sender: TObject);
    procedure CheckBox1Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure ListBox1Click(Sender: TObject);
    procedure PaintBox1Paint(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure ListViewDblClick(Sender: TObject);
    procedure Button4Click(Sender: TObject);
  private
    { Private-Deklarationen }
    FCurrentBitmap: TBitmap;
    FCurrentTexture: TJPEGTexture;
    FResultTexture: TTexture;
    procedure ShowTextures;
    procedure ShowGroundIDLinks;
  public
    { Public-Deklarationen }
    property ResultTexture: TTexture read FResultTexture;
  end;

var
  TextureManagerForm: TTextureManagerForm;

implementation

{$R *.DFM}

procedure TTextureManagerForm.Close1Click(Sender: TObject);
begin
  Close;
end;

procedure TTextureManagerForm.AddTexture1Click(Sender: TObject);
var
  AName: String;
  result: TJPEGTexture;
begin
   if OpenPictureDialog.Execute then begin
      AName := ExtractFileName(OpenPictureDialog.FileName);
      if InputQuery ('Add Texture..', 'Please enter a name:', AName) then begin
         result := pTextureManager.AddTexture (AName, OpenPictureDialog.FileName);
         if result.Name <> AName then
            MessageDlg (Format ('Warning: Texture "%s" existed: "%s"', [ AName, result.Name ]), mtWarning, [mbOk], 0);
         ShowTextures;
      end;
   end;
end;

procedure TTextureManagerForm.ShowTextures;
var
  Index: Integer;
  Texture: TJPEGTexture;
  Item: TListItem;
begin
  ListView.Items.BeginUpdate;
  ListView.Items.Clear;
  For Index := 0 to pTextureManager.Count - 1 do begin
      Texture := pTextureManager.Textures[Index];
      Item := ListView.Items.Add;
      Item.Caption := Texture.Name;
      Item.SubItems.Add (Format ('%dx%d', [ Texture.Width, Texture.Height ]));
      Item.SubItems.Add ('');
      Item.SubItems.Add (Texture.MD5Sum);
      Item.Data := Texture;

  end;
  ListView.Items.EndUpdate;
end;

procedure TTextureManagerForm.FormShow(Sender: TObject);
begin
   ShowTextures;
   FResultTexture := nil;
end;

procedure TTextureManagerForm.ListViewChange(Sender: TObject;
  Item: TListItem; Change: TItemChange);
var
  Texture: TJPEGTexture;
  Index: Integer;
begin
  FCurrentTexture := nil;
  FCurrentBitmap.Free;
  FCurrentBitmap := nil;
  if Assigned (ListView.Selected) then begin
     Texture := TJPEGTexture (ListView.Selected.Data);
     FCurrentBitmap.Free;
     FCurrentBitmap := Texture.CreateBitmap;
     Edit1.Text := Texture.Name;
     Edit2.Text := Format('%dx%d', [Texture.Width, Texture.Height]);
     Edit3.Text := Texture.MD5Sum;
     Checkbox1.Checked := Texture.HasColorKey;
     Shape1.Brush.Color := Texture.ColorKey;
     FCurrentTexture := Texture;
     ShowGroundIDLinks;
  end;

  For Index := 0 to Groupbox1.ControlCount - 1 do
      Groupbox1.Controls[Index].Enabled := Assigned (FCurrentTexture);
  For Index := 0 to Groupbox2.ControlCount - 1 do
      Groupbox2.Controls[Index].Enabled := Assigned (FCurrentTexture);

  Paintbox.Refresh;
end;

procedure TTextureManagerForm.FormDestroy(Sender: TObject);
begin
  FCurrentTexture := nil;
  FCurrentBitmap.Free;
  FCurrentBitmap := nil;
end;

procedure TTextureManagerForm.PaintBoxPaint(Sender: TObject);
begin
  if Assigned (FCurrentBitmap) then begin
     Paintbox.Canvas.Draw ((PaintBox.Width - FCurrentBitmap.Width) div 2,
                           (PaintBox.Height - FCurrentBitmap.Height) div 2,
                           FCurrentBitmap);
  end;
end;

procedure TTextureManagerForm.CheckBox1Click(Sender: TObject);
begin
  if Assigned (FCurrentTexture) then
      FCurrentTexture.HasColorKey := Checkbox1.Checked;
end;

procedure TTextureManagerForm.Button1Click(Sender: TObject);
begin
   if Assigned (FCurrentTexture) then begin
      ColorDialog.Color := FCurrentTexture.ColorKey;
      if ColorDialog.Execute then begin
         FCurrentTexture.ColorKey := ColorDialog.Color;
         Shape1.Brush.Color := ColorDialog.Color;
      end;
   end;
end;

procedure TTextureManagerForm.Button2Click(Sender: TObject);
var
  GroundID, Index: Integer;
  ATexture: TJPEGTexture;
begin
   if Assigned (FCurrentTexture) then begin
      GroundID := StrToInt(Edit5.Text);
      ATexture := pTextureManager.GetGroundMap(GroundID);
      if Assigned (ATexture) then
          if MessageDlg ('Ground ID is already linked to texture: ' + ATexture.Name + ' Do you want to overwrite this link?', mtConfirmation, [ mbYes, mbNo], 0) = mrYes then
              ATexture := nil;
      if not Assigned (ATexture) then begin
          pTextureManager.AddGroundMap(GroundID, FCurrentTexture.MD5Sum);
          ShowGroundIDLinks;
          For Index := 0 to Listbox1.Items.Count - 1 do
            if StrToInt(Listbox1.Items[Index]) = GroundID then
                Listbox1.ItemIndex := Index;
          Paintbox1.Refresh;
      end;
   end;
end;

procedure TTextureManagerForm.ShowGroundIDLinks;
var
  Index: Integer;
  List: TIntegerList;
begin
    Listbox1.Items.Clear;
    if Assigned (FCurrentTexture) then begin
        List := pTextureManager.GetLinkedGroundIDs(FCurrentTexture.MD5Sum);
        try
            For Index := 0 to List.Count - 1 do
                Listbox1.Items.Add(IntTostr(List[Index]))
        finally
            List.Free;
        end;
    end;
    Paintbox1.Refresh;
end;

procedure TTextureManagerForm.ListBox1Click(Sender: TObject);
begin
    Paintbox1.Refresh;
end;

procedure TTextureManagerForm.PaintBox1Paint(Sender: TObject);
var
  ACenter: TPoint;
begin
   if ListBox1.ItemIndex <> -1 then begin
      try
         ACenter.X := PaintBox1.ClientWidth div 2;
         ACenter.Y := PaintBox1.ClientHeight div 2;
         pArtLoader.ShowTile (StrToInt(ListBox1.Items[ListBox1.ItemIndex]), ACenter, PaintBox1.Canvas);
      except
      end;
   end;
end;

procedure TTextureManagerForm.Button3Click(Sender: TObject);
begin
   if ListBox1.ItemIndex <> -1 then begin
          pTextureManager.AddGroundMap(StrToInt(ListBox1.Items[ListBox1.ItemIndex]), '');
          ShowGroundIDLinks;
   end;
end;

procedure TTextureManagerForm.ListViewDblClick(Sender: TObject);
begin
   if Assigned (FCurrentTexture) then begin
     FResultTexture := FCurrentTexture;
     ModalResult := mrOK;
   end;
end;

procedure TTextureManagerForm.Button4Click(Sender: TObject);
var
  ADstStream: TStream;
begin
   if Assigned (FCurrentTexture) then
      if SavePictureDialog1.Execute then begin
          ADstStream := TFileStream.Create (SavePictureDialog1.FileName, fmCreate);
          try
              FCurrentTexture.Stream.Position := 0;
              ADstStream.CopyFrom(FCurrentTexture.Stream, FCurrentTexture.Stream.Size);
          finally
              ADstStream.Free;
          end;
      end;
end;

end.
