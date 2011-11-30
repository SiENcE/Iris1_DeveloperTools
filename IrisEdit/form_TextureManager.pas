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

unit form_TextureManager;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, ComCtrls, Menus, unit_TextureManager, ExtDlgs, StdCtrls, Unit_IntegerList,
  unit_ArtLoader, Unit_ModelLoader, form_TextureUsageList, JPEG;

type
  TModelShowCallback = procedure (AModelID: Integer) of object;

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
    PopupMenu: TPopupMenu;
    Rename1: TMenuItem;
    Remove1: TMenuItem;
    Exchange1: TMenuItem;
    UsageList1: TMenuItem;
    AddTexture2: TMenuItem;
    SavePictureDialog: TSavePictureDialog;
    ExportTexture1: TMenuItem;
    procedure UsageList1Click(Sender: TObject);
    procedure ExportTexture1Click(Sender: TObject);
    procedure Exchange1Click(Sender: TObject);
    procedure PopupMenuPopup(Sender: TObject);
    procedure Remove1Click(Sender: TObject);
    procedure Rename1Click(Sender: TObject);
    procedure ListViewCompare(Sender: TObject; Item1, Item2: TListItem;
      Data: Integer; var Compare: Integer);
    procedure FormCreate(Sender: TObject);
    procedure ListViewColumnClick(Sender: TObject; Column: TListColumn);
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
  private
    { Private-Deklarationen }
    FCurrentBitmap: TBitmap;
    FCurrentTexture: TJPEGTexture;
    FResultTexture: TTexture;
    FSortIndex: Integer;
    FSortDirection: Integer;
    FIsModal: Boolean;
    FModelShowCallback: TModelShowCallback;
    procedure ShowTextures;
    procedure ShowGroundIDLinks;
  public
    { Public-Deklarationen }
    property ResultTexture: TTexture read FResultTexture;
    property IsModal: Boolean read FIsModal write FIsModal;
    property ModelShowCallback: TModelShowCallback read FModelShowCallback write FModelShowCallback;
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
  pModelLoader.CountTextureUsage;

  ListView.Items.BeginUpdate;
  ListView.Items.Clear;
  For Index := 0 to pTextureManager.Count - 1 do begin
      Texture := pTextureManager.Textures[Index];
      Item := ListView.Items.Add;
      Item.Caption := Texture.Name;
      Item.SubItems.Add (Format ('%dx%d', [ Texture.Width, Texture.Height ]));
      Item.SubItems.Add (IntToStr (Texture.UseCount));
      Item.SubItems.Add (Texture.MD5Sum);
      Item.Data := Texture;

  end;
  ListView.Items.EndUpdate;
end;

procedure TTextureManagerForm.FormShow(Sender: TObject);
begin
   ShowTextures;
   StatusBar1.SimpleText := Format ('%d textures loaded', [pTextureManager.Count]);
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
     Edit4.Text := IntToStr (Texture.UseCount);
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
  Paintbox.Canvas.Brush.Color := clGray;
  Paintbox.Canvas.FillRect (Paintbox.Canvas.ClipRect);
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
  if FIsModal then begin
     if Assigned (FCurrentTexture) then begin
         FResultTexture := FCurrentTexture;
         ModalResult := mrOK;
       end;
  end else
      UsageList1Click (Sender);
end;

procedure TTextureManagerForm.ListViewColumnClick(Sender: TObject;
  Column: TListColumn);
begin
    if FSortIndex <> Column.Index then
        FSortDirection := 1
    else
        FSortDirection := - FSortDirection;
    FSortIndex := Column.Index;
    ListView.AlphaSort;
end;

procedure TTextureManagerForm.FormCreate(Sender: TObject);
begin
    FSortIndex := -1;
    Paintbox.ControlStyle := Paintbox.ControlStyle + [csOpaque];
end;

procedure TTextureManagerForm.ListViewCompare(Sender: TObject; Item1,
  Item2: TListItem; Data: Integer; var Compare: Integer);
var
    Texture1, Texture2: TJPEGTexture;
begin
    Texture1 := Item1.Data;
    Texture2 := Item2.Data;

    Compare := 0;

        case FSortIndex of
            1: begin
                  if (Texture1.Width * Texture1.Height) < (Texture2.Width * Texture2.Height) then Compare := -1;
                  if (Texture1.Width * Texture1.Height) > (Texture2.Width * Texture2.Height) then Compare := 1;
               end;
            2: begin
                  if Texture1.UseCount > Texture2.UseCount then Compare := -1;
                  if Texture1.UseCount < Texture2.UseCount then Compare := 1;
               end;
            3: begin
                 if AnsiUpperCase(Texture1.MD5Sum) < AnsiUpperCase (Texture2.MD5Sum) then Compare := -1;
                 if AnsiUpperCase(Texture1.MD5Sum) > AnsiUpperCase (Texture2.MD5Sum) then Compare := 1;
               end;
        end;

    if Compare = 0 then begin
       if AnsiUpperCase(Texture1.Name) < AnsiUpperCase (Texture2.Name) then Compare := -1;
       if AnsiUpperCase(Texture1.Name) > AnsiUpperCase (Texture2.Name) then Compare := 1;
    end;

    Compare := Compare * FSortDirection;
end;

procedure TTextureManagerForm.Rename1Click(Sender: TObject);
var
    S: String;
begin
    if Assigned (FCurrentTexture) then begin
        S := InputBox ('Rename Texture', 'Name: ', FCurrentTexture.Name);
        if S <> FCurrentTexture.Name then
            FCurrentTexture.Name := S;
        if Assigned (ListView.Selected) then
            ListView.Selected.Caption := S;
    end;
end;

procedure TTextureManagerForm.Remove1Click(Sender: TObject);
begin
    if Assigned (FCurrentTexture) then begin
          pModelLoader.CountTextureUsage;
          if FCurrentTexture.UseCount > 0 then
              raise Exception.Create ('Texture is in use - you can not remove it');
          pTextureManager.RemoveTexture (FCurrentTexture.MD5Sum);
          FCurrentTexture := nil;
          if Assigned (ListView.Selected) then
              ListView.Items.Delete (ListView.Selected.Index);
    end;

end;

procedure TTextureManagerForm.PopupMenuPopup(Sender: TObject);
begin
    Rename1.Enabled := Assigned (FCurrentTexture);
    Remove1.Enabled := Assigned (FCurrentTexture);
    Exchange1.Enabled := Assigned (FCurrentTexture);
    ExportTexture1.Enabled := Assigned (FCurrentTexture);
    UsageList1.Enabled := Assigned (FCurrentTexture);
    UsageList1.Default := not FIsModal;
end;

procedure TTextureManagerForm.Exchange1Click(Sender: TObject);
var
  AName: String;
  result: TJPEGTexture;
begin
   if Assigned (FCurrentTexture) then begin
       pModelLoader.CountTextureUsage;
       if FCurrentTexture.UseCount = 0 then
              raise Exception.Create ('Texture is not in use - can not exchange anything');

       if OpenPictureDialog.Execute then begin
          AName := ExtractFileName(OpenPictureDialog.FileName);
          if InputQuery ('Exchange Texture..', 'Please enter a name:', AName) then begin
               result := pTextureManager.AddTexture (AName, OpenPictureDialog.FileName);
               pModelLoader.ExchangeTexture (FCurrentTexture, result);
               FCurrentTexture := nil;
               if result.Name <> AName then
                  MessageDlg (Format ('Warning: Texture "%s" existed: "%s"', [ AName, result.Name ]), mtWarning, [mbOk], 0);
               ShowTextures;
            end;
       end;
   end;
end;

procedure TTextureManagerForm.ExportTexture1Click(Sender: TObject);
var
    ABitmap: TBitmap;
    AJPEGImage: TJPEGImage;
begin
   if Assigned (FCurrentTexture) then
      if SavePictureDialog.Execute then begin
          ABitmap := FCurrentTexture.CreateBitmap;
          try
              AJPEGImage := TJPEGImage.Create;
              try
                  AJPEGImage.CompressionQuality := 94;
                  AJPEGImage.Assign (ABitmap);
                  AJPEGImage.SaveToFile (SavePictureDialog.FileName);
              finally
                  AJPEGImage.Free;
              end;
          finally
              ABitmap.Free;
          end;
      end;

end;

procedure TTextureManagerForm.UsageList1Click(Sender: TObject);
var
    Form: TTextureUsageForm;
begin
   if Assigned (FCurrentTexture) then begin
       Form := TTextureUsageForm.Create (Self, FCurrentTexture);
       try
          if Form.ShowModal = mrOK then begin
              if Assigned (FModelShowCallback) then
                  FModelShowCallback (Form.ResultModelID);
          end;
       finally
          Form.Free;
       end;
   end;

end;

end.
