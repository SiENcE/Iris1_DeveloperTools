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

unit form_Main;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Menus, ComCtrls, StdCtrls, Unit_TileDataLoader, Unit_Settings, ExtCtrls, DeCAL,
  contnrs, Unit_ArtLoader, Unit_Models, Unit_ModelLoader, form_Edit, FileCtrl,
  Unit_OpenGL, form_Progress, Unit_TextureManager, ExtDlgs, form_Add3DS,
  form_Info;

const
  VersionString = '0.95';

type
  TCategoryEntry = class (TObject)
      Index: Integer;
      Name: String;
      Count: Integer;
      Done: Integer;
      SortKey: String;
  end;

  TItemEntry = class (TObject)
      Category: TCategoryEntry;
      ID: Integer;
      Model: TModel;
  end;

  TMainForm = class(TForm)
    MainMenu1: TMainMenu;
    Models1: TMenuItem;
    Save1: TMenuItem;
    Load1: TMenuItem;
    Textures1: TMenuItem;
    Managetextures1: TMenuItem;
    N1: TMenuItem;
    Exit1: TMenuItem;
    Options1: TMenuItem;
    Settings1: TMenuItem;
    Import1: TMenuItem;
    CategoryListView: TListView;
    ListView: TListView;
    Splitter1: TSplitter;
    StatusBar: TStatusBar;
    Panel1: TPanel;
    ArtPreviewBox: TPaintBox;
    Splitter2: TSplitter;
    N2: TMenuItem;
    FindTile1: TMenuItem;
    ImportDialog: TOpenDialog;
    CreateGameFiles1: TMenuItem;
    N3: TMenuItem;
    PopupMenu1: TPopupMenu;
    DeleteModel1: TMenuItem;
    RecreateModel1: TMenuItem;
    SaveDialog: TSaveDialog;
    UpdateGameFile1: TMenuItem;
    ApplyFlags1: TMenuItem;
    SetTileable1: TMenuItem;
    UnsetTileableflag1: TMenuItem;
    SetCastsShadowFlag1: TMenuItem;
    UnsetCastsShadowFlag1: TMenuItem;
    Import3DS1: TMenuItem;
    OpenDialog3DS: TOpenDialog;
    OpenPictureDialog: TOpenPictureDialog;
    About1: TMenuItem;
    procedure FormShow(Sender: TObject);
    procedure About1Click(Sender: TObject);
    procedure Settings1Click(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure ArtPreviewBoxPaint(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure CategoryListViewChange(Sender: TObject; Item: TListItem;
      Change: TItemChange);
    procedure ListViewChange(Sender: TObject; Item: TListItem;
      Change: TItemChange);
    procedure FindTile1Click(Sender: TObject);
    procedure Import1Click(Sender: TObject);
    procedure ListViewDblClick(Sender: TObject);
    procedure CategoryListViewColumnClick(Sender: TObject;
      Column: TListColumn);
    procedure CategoryListViewCompare(Sender: TObject; Item1,
      Item2: TListItem; Data: Integer; var Compare: Integer);
    procedure Managetextures1Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure ListViewColumnClick(Sender: TObject; Column: TListColumn);
    procedure ListViewCompare(Sender: TObject; Item1, Item2: TListItem;
      Data: Integer; var Compare: Integer);
    procedure CreateGameFiles1Click(Sender: TObject);
    procedure UpdateGameFile1Click(Sender: TObject);
    procedure Import3DS1Click(Sender: TObject);
    procedure DeleteModel1Click(Sender: TObject);
    procedure RecreateModel1Click(Sender: TObject);
    procedure SetTileable1Click(Sender: TObject);
  private
    { Private-Deklarationen }
    FCategories: TObjectList;
    FItems: TObjectList;
    FArtIsInvalid: Boolean;
    FCurrentCategory: TCategoryEntry;
    Initialized: Boolean;
    FCategoryReverseSort: Boolean;
    FCategorySortIndex: Integer;
    FItemReverseSort: Boolean;
    FItemSortIndex: Integer;
    procedure UpdateLists;
    procedure UpdateStatus;
    procedure CreateCategories;
    procedure OpenModel (AModelID: Integer);
  public
    { Public-Deklarationen }
  end;

var
  MainForm: TMainForm;

implementation

uses form_Settings, form_TextureManager;

{$R *.DFM}

procedure TMainForm.Settings1Click(Sender: TObject);
begin
   SettingsForm.ShowModal;
end;

procedure TMainForm.Exit1Click(Sender: TObject);
begin
   Close;
end;

procedure TMainForm.FormCreate(Sender: TObject);
var
  FileName: String;
begin
   LoadSettings;

   Caption := Caption + ' ' + VersionString;

   try
       pTileDataLoader := TTileDataLoader.Create (TileDataMul);
       pArtLoader := TArtLoader.Create(ArtMul, ArtIdxMul);
       FileName := ExtractFilePath(Application.ExeName) + 'IrisEdit.imc';
       if FileExists (FileName) then
            pModelLoader.Load (FileName);
            
       Initialized := True;
   except
   end;

   CreateCategories;
end;

procedure TMainForm.ArtPreviewBoxPaint(Sender: TObject);
var
  ACenter: TPoint;
  TileID: Integer;
begin
   if Assigned(pArtLoader) and Assigned(ListView.Selected) and not FArtIsInvalid then begin
      TileID := TItemEntry (ListView.Selected.Data).ID;
      ACenter.X := ArtPreviewBox.ClientWidth div 2;
      ACenter.Y := ArtPreviewBox.ClientHeight div 2;
      try
         pArtLoader.ShowTile (16384 + TileID, ACenter, ArtPreviewBox.Canvas);
         StatusBar.Panels[1].Text := Format('model #%d', [TileID]);
      except
         On E: Exception do begin
            StatusBar.Panels[1].Text := E.Message;
            FArtIsInvalid := True;
         end;
      end;
   end;

   if FArtIsInvalid then
       with ArtPreviewBox.Canvas do begin
           Brush.Color := clGray;
           Brush.Style := bsCross;
           FillRect (ClipRect);
           Brush.Color := clBtnFace;
           Brush.Style := bsSolid;
       end;
end;

procedure TMainForm.FormDestroy(Sender: TObject);
begin
   FItems.Free;
   FItems := nil;
   FCategories.Free;
   FCategories := nil;
end;

procedure TMainForm.CategoryListViewChange(Sender: TObject;
  Item: TListItem; Change: TItemChange);
var
   Index: Integer;
   ItemEntry: TItemEntry;
   Category: TCategoryEntry;
   ListItem: TListItem;
begin
   if not Assigned (FItems) then
       exit;

   ListView.Items.Clear;

   if Assigned(Item) then
     if Item.Data = FCurrentCategory then
       Exit;


   ListView.Selected := nil;
   ListView.Items.BeginUpdate;
   FCurrentCategory := nil;
   if Assigned (CategoryListView.Selected) then begin
      Category := TCategoryEntry (CategoryListView.Selected.Data);
      FCurrentCategory := Category;
      ListView.AllocBy := FItems.Count;
      For Index := 0 to FItems.Count - 1 do begin
          ItemEntry := FItems[Index] as TItemEntry;
          if ItemEntry.Category = Category then begin
               ListItem := ListView.Items.Add;
               ListItem.Data := ItemEntry;
               ListItem.Caption := IntToStr (ItemEntry.ID);
               ListItem.SubItems.Add (Category.Name);
               if Assigned(ItemEntry.Model) then begin
                  ListItem.SubItems.Add (IntToStr(ItemEntry.Model.NodeCount));
                  ListItem.SubItems.Add (IntToStr(ItemEntry.Model.FaceCount));
                  ListItem.SubItems.Add(ItemEntry.Model.GetFlagString);
               end else begin
                  ListItem.SubItems.Add ('---');
                  ListItem.SubItems.Add ('---');
                  ListItem.SubItems.Add ('---');
               end;
          end;
      end;
   end;
   ListView.Items.EndUpdate;

   if ListView.Items.Count > 0 then
      ListView.Selected := ListView.Items[0];

end;

procedure TMainForm.ListViewChange(Sender: TObject; Item: TListItem;
  Change: TItemChange);
begin
   if Change = ctState then begin
      FArtIsInvalid := False; // Reset invalid flag;
      ArtPreviewBox.Refresh;
   end;
end;

procedure TMainForm.FindTile1Click(Sender: TObject);
var
  S: String;
  ID: Integer;
  Index, TileIndex: Integer;
  Item: TItemEntry;
begin
  S := InputBox ('Find Tile', 'Tile ID:', '');
  ID := StrToIntDef (S, -1);
  if (ID >= 0) and (ID < FItems.Count) then begin
     Item := FItems[ID] as TItemEntry;
     For Index := 0 to CategoryListView.Items.Count - 1 do
         if CategoryListView.Items[Index].Data = Item.Category then begin
            CategoryListView.Selected := CategoryListView.Items[Index];
            For TileIndex := 0 to ListView.Items.Count - 1 do
               if ListView.Items[TileIndex].Data = Item then begin
                  ListView.Selected := ListView.Items[TileIndex];
                  break;
               end;

            break;
         end;

  end;
end;

procedure TMainForm.Import1Click(Sender: TObject);
var
  TextureDir: String;
  ProgressForm: TProgressForm;
begin
   if ImportDialog.Execute then begin
      TextureDir := '';
      if SelectDirectory ('Please choose a texture directory', '', TextureDir) then begin
          ProgressForm := TProgressForm.Create (nil);
          ProgressForm.Left := Left + (Width - ProgressForm.Width) div 2;
          ProgressForm.Top := Top + (Height - ProgressForm.Height) div 2;
          ProgressForm.Show;
          ProgressForm.InfoText.Caption := 'Importing ' + ImportDialog.FileName;
          ProgressForm.Infotext.Update;
          try
              pModelLoader.Import (ImportDialog.Filename, TextureDir, ProgressForm.SetStatus);
          finally
             ProgressForm.Free;
          end;
          UpdateLists;
          UpdateStatus;
      end;
   end;
end;

procedure TMainForm.UpdateLists;
var
  Index: Integer;
  Item: TItemEntry;
  Category: TCategoryEntry;
begin
  Assert (Assigned (pModelLoader));

  For Index := 0 to FCategories.Count - 1 do
      (FCategories[Index] as TCategoryEntry).Done := 0;

  For Index := 0 to FItems.Count - 1 do begin
      Item := FItems[Index] as TItemEntry;
      Item.Model := pModelLoader.Get(Index);
      if Assigned(Item.Model) then
         inc (Item.Category.Done);
  end;

  CategoryListView.Items.BeginUpdate;
  For Index := 0 to CategoryListView.Items.Count - 1 do begin
     Category := TCategoryEntry(CategoryListView.Items[Index].Data);
     CategoryListView.Items[Index].SubItems[1] :=
         IntToStr(Category.Done);
     CategoryListView.Items[Index].SubItems[2] :=
         IntToStr(Category.Count - Category.Done);
  end;
  CategoryListView.Items.EndUpdate;

  ListView.Items.BeginUpdate;
  For Index := 0 to CategoryListView.Items.Count - 1 do
     CategoryListView.Items[Index].SubItems[1] :=
         IntToStr(TCategoryEntry(CategoryListView.Items[Index].Data).Done);
  ListView.Items.EndUpdate;
end;

procedure TMainForm.ListViewDblClick(Sender: TObject);
var
  Item: TItemEntry;
//  Form: TEditForm;
begin
  if Assigned (ListView.Selected) then begin
     Item := TItemEntry (ListView.Selected.Data);
     OpenModel (Item.ID);
{     if not Assigned (Item.Model) then
       if MessageDlg (Format ('Model #%d (%s) does not exist. Do you want to create it?', [ Item.ID, Item.Category.Name ]), mtConfirmation, [ mbYes, mbNo ], 0) = mrYes then begin
          Item.Model := TModel.Create (Item.ID);
          pModelLoader.Add (Item.ID, Item.Model);
          UpdateStatus;
          UpdateLists;
          CategoryListViewChange (Sender, nil, ctText);
       end;

     if Assigned (Item.Model) then begin
        Form := TEditForm.Create(Item.Model, Item.ID, Item.Category.Name, Self);
        Form.Show;
     end;}
  end;
end;

procedure TMainForm.CategoryListViewColumnClick(Sender: TObject;
  Column: TListColumn);
begin
  if Column.Index = FCategorySortIndex then begin
     FCategoryReverseSort := not FCategoryReverseSort;
  end else begin
     FCategorySortIndex := Column.Index;
     FCategoryReverseSort := False;
  end;
  CategoryListView.AlphaSort;
end;

procedure TMainForm.CategoryListViewCompare(Sender: TObject; Item1,
  Item2: TListItem; Data: Integer; var Compare: Integer);
var
  Category1, Category2: TCategoryEntry;
begin
  Compare := 1;
  Category1 := TCategoryEntry (Item1.Data);
  Category2 := TCategoryEntry (Item2.Data);
  case FCategorySortIndex of
       0: if Category1.SortKey < Category2.SortKey then Compare := -1;
       1: if Category1.Count < Category2.Count then Compare := -1;
       2: if Category1.Done < Category2.Done then Compare := -1;
       3: if (Category1.Count - Category1.Done) < (Category2.Count - Category2.Done) then Compare := -1;
  end;

  if FCategoryReverseSort then
        Compare := - Compare;
end;

procedure TMainForm.UpdateStatus;
begin
  if Assigned (pModelLoader) then
     StatusBar.Panels[0].Text := Format ('%d models loaded', [ pModelLoader.Count ])
  else
     StatusBar.Panels[0].Text := '';
end;

procedure TMainForm.Managetextures1Click(Sender: TObject);
begin
   TextureManagerForm.IsModal := False;
   TextureManagerForm.Show;
end;

procedure TMainForm.FormClose(Sender: TObject; var Action: TCloseAction);
begin
   if Initialized then
     pModelLoader.Save (ExtractFilePath(Application.ExeName) + 'IrisEdit.imc');
end;

procedure TMainForm.CreateCategories;
var
  Index: Integer;
  Item: TListItem;
  Iter: DIterator;
  Map: DMap;
  S: String;
  Category: TCategoryEntry;
  ItemEntry: TItemEntry;
begin
   FCategories := TObjectList.Create;
   FItems := TObjectList.Create;

   Map := DMap.Create;
   try
      For Index := 0 to pTileDataLoader.Count - 1 do begin
          S := AnsiUpperCase(Trim(pTileDataLoader[Index].Name));

          Iter := Map.locate ([S]);
          if not atEnd (Iter) then begin
              Category := getObject(Iter) as TCategoryEntry;
              inc (Category.Count);
          end else begin
              Category := TCategoryEntry.Create;
              if S <> '' then
                 Category.Name := Trim (pTileDataLoader[Index].Name)
              else
                 Category.Name := '<unnamed>';
              Category.Count := 1;
              Category.SortKey := S;
              Map.putPair ([S, Category]);
          end;

          ItemEntry := TItemEntry.Create;
          ItemEntry.ID := Index;
          ItemEntry.Category := Category;
          ItemEntry.Model := pModelLoader.Get(Index);
          if Assigned (ItemEntry.Model) then
             inc (Category.Done);
          FItems.Add (ItemEntry);
      end;

      Iter := Map.Start; Index := 0;
      while not atEnd (Iter) do begin
            Category := getObject(Iter) as TCategoryEntry;
            Category.Index := Index;
            FCategories.Add (Category);
            Advance (Iter);
            inc (Index);
      end;

   finally
      Map.Free;
   end;

   CategoryListView.Items.BeginUpdate;
   CategoryListView.AllocBy := FCategories.Count;
   For Index := 0 to FCategories.Count - 1 do begin
      Item := CategoryListView.Items.Add;
      Category := FCategories[Index] as TCategoryEntry;
      if Category.Name = '' then
          Item.Caption := '<unnamed>'
      else
          Item.Caption := Category.Name;
      Item.Data := Category;
      Item.SubItems.Add (IntToStr(Category.Count));
      Item.SubItems.Add (IntToStr(Category.Done));
      Item.SubItems.Add (IntToStr(Category.Count - Category.Done));
   end;
   CategoryListView.Items.EndUpdate;

   UpdateStatus;
end;

procedure TMainForm.ListViewColumnClick(Sender: TObject;
  Column: TListColumn);
begin
  if Column.Index = FItemSortIndex then begin
     FItemReverseSort := not FItemReverseSort;
  end else begin
     FItemSortIndex := Column.Index;
     FItemReverseSort := False;
  end;
  ListView.AlphaSort;
end;

procedure TMainForm.ListViewCompare(Sender: TObject; Item1,
  Item2: TListItem; Data: Integer; var Compare: Integer);
var
  ItemEntry1, ItemEntry2: TItemEntry;
begin
  Compare := 1;
  ItemEntry1 := TItemEntry (Item1.Data);
  ItemEntry2 := TItemEntry (Item2.Data);
  case FItemSortIndex of
       0: if ItemEntry1.ID < ItemEntry2.ID then Compare := -1;
       1: if ItemEntry1.Category.SortKey < ItemEntry2.Category.SortKey then Compare := -1;
       2: begin
               if Assigned (ItemEntry2.Model) and not Assigned (ItemEntry1.Model) then
                  Compare := -1;
               if Assigned (ItemEntry1.Model) and Assigned (ItemEntry2.Model) then
                  if ItemEntry1.Model.NodeCount < ItemEntry2.Model.NodeCount then
                     Compare := -1
          end;
       3: begin
               if Assigned (ItemEntry2.Model) and not Assigned (ItemEntry1.Model) then
                  Compare := -1;
               if Assigned (ItemEntry1.Model) and Assigned (ItemEntry2.Model) then
                  if ItemEntry1.Model.FaceCount < ItemEntry2.Model.FaceCount then
                     Compare := -1
          end;
  end;

  if FItemReverseSort then
        Compare := - Compare;

end;

procedure TMainForm.CreateGameFiles1Click(Sender: TObject);
var
   ProgressForm: TProgressForm;
   PatchXML: TStrings;
   PatchXMLFileName: String;
   SavePatch: Boolean;
begin
  if SaveDialog.Execute then begin
          ProgressForm := TProgressForm.Create (nil);
          ProgressForm.Left := Left + (Width - ProgressForm.Width) div 2;
          ProgressForm.Top := Top + (Height - ProgressForm.Height) div 2;
          ProgressForm.Show;
          ProgressForm.InfoText.Caption := 'Creating Game File...';
          ProgressForm.Infotext.Update;
          PatchXML := TStringList.Create;
          try
             pModelLoader.CreateGameFile (SaveDialog.Filename, ProgressForm.SetStatus, PatchXML);
             PatchXMLFileName := copy (SaveDialog.Filename, 1, Length (SaveDialog.Filename) - Length(ExtractFileExt(SaveDialog.Filename))) + '-patchinfo.xml';
             if FileExists(PatchXMLFileName) then
                SavePatch := MessageDlg ('overwrite ' + PatchXMLFileName + '?', mtConfirmation, [mbYes, mbNo], 0) = mrYes
             else
                SavePatch := True;
             if SavePatch then
                 PatchXML.SaveToFile(PatchXMLFileName);
          finally
             ProgressForm.Free;
             PatchXML.Free;
          end;
  end;
end;

procedure TMainForm.UpdateGameFile1Click(Sender: TObject);
var
   ProgressForm: TProgressForm;
   PatchXML: TStrings;
   PatchXMLFileName: String;
   SavePatch: Boolean;
begin
  if SaveDialog.Execute then begin
          ProgressForm := TProgressForm.Create (nil);
          ProgressForm.Left := Left + (Width - ProgressForm.Width) div 2;
          ProgressForm.Top := Top + (Height - ProgressForm.Height) div 2;
          ProgressForm.Show;
          ProgressForm.InfoText.Caption := 'Updating Game File...';
          ProgressForm.Infotext.Update;
          PatchXML := TStringList.Create;
          try
             pModelLoader.UpdateGameFile (SaveDialog.Filename, ProgressForm.SetStatus, PatchXML);
             PatchXMLFileName := copy (SaveDialog.Filename, 1, Length (SaveDialog.Filename) - Length(ExtractFileExt(SaveDialog.Filename))) + '-patchinfo.xml';
             if FileExists(PatchXMLFileName) then
                SavePatch := MessageDlg ('overwrite ' + PatchXMLFileName + '?', mtConfirmation, [mbYes, mbNo], 0) = mrYes
             else
                SavePatch := True;
             if SavePatch then
                 PatchXML.SaveToFile(PatchXMLFileName);
          finally
             ProgressForm.Free;
             PatchXML.Free;
          end;
  end;
end;

procedure TMainForm.Import3DS1Click(Sender: TObject);
var
  Item: TItemEntry;
  EditForm: TEditForm;
  Form: TImport3DSForm;
  AName: String;
  ATexture: TJPEGTexture;
begin
  try
    if Assigned (ListView.Selected) then begin
       Item := TItemEntry (ListView.Selected.Data);

    Form := TImport3DSForm.Create (Self);
    try
        if Form.ShowModal = mrOK then begin
            if (Form.FileName = '') or not FileExists (Form.FileName) then
                raise Exception.Create ('File does not exist');
            if Assigned(Item.Model) then
                pModelLoader.Remove(Item.ID);
            Item.Model := nil;

            Item.Model := TModel.CreateFrom3DS(Item.ID, Form.FileName, Form.ScaleMode, Form.ScaleFactor, Form.TranslateToDefaultPos);
            pModelLoader.Add(Item.ID, Item.Model);

                if (Form.TextureFileName <> '') and FileExists (Form.TextureFileName) then begin
                    AName := ExtractFileName(Form.TextureFileName);
                    if InputQuery ('Add Texture..', 'Please enter a name:', AName) then begin
                         ATexture := pTextureManager.AddTexture (AName, Form.TextureFileName);
                         if ATexture.Name <> AName then
                              MessageDlg (Format ('Warning: Texture "%s" existed: "%s"', [ AName, ATexture.Name ]), mtWarning, [mbOk], 0);
                         Item.Model.ApplyTexture(ATexture, False);
                    end;
                end;
            EditForm := TEditForm.Create(Item.Model, Item.ID, Item.Category.Name, Self);
            EditForm.Show;
        end;
    finally
        Form.Free;
    end;


{            Item.Model := TModel.CreateFrom3DS(Item.ID, OpenDialog3DS.FileName);
            pModelLoader.Add(Item.ID, Item.Model);

            if OpenPictureDialog.Execute then begin
                AName := ExtractFileName(OpenPictureDialog.FileName);
                if InputQuery ('Add Texture..', 'Please enter a name:', AName) then begin
                     ATexture := pTextureManager.AddTexture (AName, OpenPictureDialog.FileName);
                     if ATexture.Name <> AName then
                          MessageDlg (Format ('Warning: Texture "%s" existed: "%s"', [ AName, ATexture.Name ]), mtWarning, [mbOk], 0);
                     Item.Model.ApplyTexture(ATexture, False);
                end;
            end;}

//       end;
    end;
  finally
      CategoryListViewChange (Sender, nil, ctText);
  end;
end;

procedure TMainForm.DeleteModel1Click(Sender: TObject);
var
  Item: TItemEntry;
  Index: Integer;
begin
  if MessageDlg ('Are you sure?', mtConfirmation, [mbYes, mbNo], 0)  = mrYes then
    for Index := 0 to ListView.Items.Count - 1 do
      if ListView.Items[Index].Selected then begin
         Item := TItemEntry (ListView.Items[Index].Data);
         if Assigned (Item.Model) then
              pModelLoader.Remove(Item.ID);
         Item.Model := nil;
      end;

  CategoryListViewChange (Sender, nil, ctText);
end;

procedure TMainForm.RecreateModel1Click(Sender: TObject);
var
  Item: TItemEntry;
  Form: TEditForm;
begin
  if MessageDlg ('Are you sure?', mtConfirmation, [mbYes, mbNo], 0)  = mrYes then
  try
    if Assigned (ListView.Selected) then begin
       Item := TItemEntry (ListView.Selected.Data);
       if Assigned(Item.Model) then
            pModelLoader.Remove(Item.ID);
       Item.Model := TModel.Create(Item.ID);
       pModelLoader.Add(Item.ID, Item.Model);

       Form := TEditForm.Create(Item.Model, Item.ID, Item.Category.Name, Self);
       Form.Show;
    end;
  finally
      CategoryListViewChange (Sender, nil, ctText);
  end;
end;

procedure TMainForm.SetTileable1Click(Sender: TObject);
var
  Item: TItemEntry;
  Index: Integer;
begin
    for Index := 0 to ListView.Items.Count - 1 do
      if ListView.Items[Index].Selected then begin
         Item := TItemEntry (ListView.Items[Index].Data);
         if Assigned (Item.Model) then begin
              pModelLoader.LockModel(Item.ID);
              if Sender = SetTileable1 then
                  Item.Model.Flags := Item.Model.Flags + [ mfModelIsTileable ];
              if Sender = UnsetTileableFlag1 then
                  Item.Model.Flags := Item.Model.Flags - [ mfModelIsTileable ];
              if Sender = SetCastsShadowFlag1 then
                  Item.Model.Flags := Item.Model.Flags + [ mfModelCastsShadows ];
              if Sender = UnsetCastsShadowFlag1 then
                  Item.Model.Flags := Item.Model.Flags - [ mfModelCastsShadows ];
              Item.Model.LastChange := Now;
              pModelLoader.UnlockModel(Item.ID);
         end;
      end;

  CategoryListViewChange (Sender, nil, ctText);
end;

procedure TMainForm.About1Click(Sender: TObject);
var
    InfoForm: TInfoForm;
begin
    InfoForm := TInfoForm.Create (Self);
    try
        InfoForm.ShowModal;
    finally
        InfoForm.Free;
    end;
end;

procedure TMainForm.OpenModel (AModelID: Integer);
var
  Item: TItemEntry;
  Form: TEditForm;
begin
    Assert (AModelID >= 0);
    if AModelID >= FItems.Count then
        raise Exception.Create ('Model not found');

   Item := FItems[AModelID] as TItemEntry;
     if not Assigned (Item.Model) then
       if MessageDlg (Format ('Model #%d (%s) does not exist. Do you want to create it?', [ Item.ID, Item.Category.Name ]), mtConfirmation, [ mbYes, mbNo ], 0) = mrYes then begin
          Item.Model := TModel.Create (Item.ID);
          pModelLoader.Add (Item.ID, Item.Model);
          UpdateStatus;
          UpdateLists;
          CategoryListViewChange (nil, nil, ctText);
       end;

     if Assigned (Item.Model) then begin
        Form := TEditForm.Create(Item.Model, Item.ID, Item.Category.Name, Self);
        Form.Show;
     end;
end;

procedure TMainForm.FormShow(Sender: TObject);
begin
   TextureManagerForm.ModelShowCallback := OpenModel;
end;

end.
