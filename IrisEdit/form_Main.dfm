object MainForm: TMainForm
  Left = 44
  Top = 114
  Width = 850
  Height = 651
  Caption = 'Iris - Model Editor'
  Color = clBtnFace
  Constraints.MinHeight = 450
  Constraints.MinWidth = 850
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Splitter1: TSplitter
    Left = 289
    Top = 0
    Height = 586
  end
  object Splitter2: TSplitter
    Left = 574
    Top = 0
    Width = 4
    Height = 586
    Align = alRight
  end
  object CategoryListView: TListView
    Left = 0
    Top = 0
    Width = 289
    Height = 586
    Align = alLeft
    Columns = <
      item
        AutoSize = True
        Caption = 'Category'
      end
      item
        Caption = 'Count'
      end
      item
        Caption = 'done'
      end
      item
        Caption = 'to do'
        Width = 70
      end>
    GridLines = True
    HideSelection = False
    ReadOnly = True
    RowSelect = True
    SortType = stData
    TabOrder = 0
    ViewStyle = vsReport
    OnChange = CategoryListViewChange
    OnColumnClick = CategoryListViewColumnClick
    OnCompare = CategoryListViewCompare
  end
  object ListView: TListView
    Left = 292
    Top = 0
    Width = 282
    Height = 586
    Align = alClient
    Columns = <
      item
        Caption = 'TileID'
      end
      item
        AutoSize = True
        Caption = 'Description'
      end
      item
        Caption = 'Points'
      end
      item
        Caption = 'Faces'
      end
      item
        Caption = 'Flags'
      end>
    GridLines = True
    HideSelection = False
    MultiSelect = True
    ReadOnly = True
    RowSelect = True
    PopupMenu = PopupMenu1
    TabOrder = 1
    ViewStyle = vsReport
    OnChange = ListViewChange
    OnColumnClick = ListViewColumnClick
    OnCompare = ListViewCompare
    OnDblClick = ListViewDblClick
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 586
    Width = 842
    Height = 19
    Panels = <
      item
        Text = '3210 models loaded'
        Width = 150
      end
      item
        Width = 100
      end>
  end
  object Panel1: TPanel
    Left = 578
    Top = 0
    Width = 264
    Height = 586
    Align = alRight
    BevelOuter = bvNone
    BorderStyle = bsSingle
    TabOrder = 3
    object ArtPreviewBox: TPaintBox
      Left = 0
      Top = 0
      Width = 260
      Height = 582
      Align = alClient
      OnPaint = ArtPreviewBoxPaint
    end
  end
  object MainMenu1: TMainMenu
    Left = 448
    Top = 24
    object Models1: TMenuItem
      Caption = '&Models'
      object Save1: TMenuItem
        Caption = '&Save...'
      end
      object Load1: TMenuItem
        Caption = '&Load...'
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object Import1: TMenuItem
        Caption = '&Import Pre-0.6 Format..'
        OnClick = Import1Click
      end
      object CreateGameFiles1: TMenuItem
        Caption = 'Create Game File...'
        OnClick = CreateGameFiles1Click
      end
      object UpdateGameFile1: TMenuItem
        Caption = 'Update Game File...'
        OnClick = UpdateGameFile1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object FindTile1: TMenuItem
        Caption = '&Find Tile...'
        ShortCut = 114
        OnClick = FindTile1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = '&Exit'
        OnClick = Exit1Click
      end
    end
    object Textures1: TMenuItem
      Caption = '&Textures'
      object Managetextures1: TMenuItem
        Caption = 'Manage textures...'
        OnClick = Managetextures1Click
      end
    end
    object Options1: TMenuItem
      Caption = '&Options'
      object Settings1: TMenuItem
        Caption = '&Settings...'
        OnClick = Settings1Click
      end
      object About1: TMenuItem
        Caption = '&About..'
        OnClick = About1Click
      end
    end
  end
  object ImportDialog: TOpenDialog
    DefaultExt = 'dat'
    FileName = 'models.dat'
    Filter = 'Old Model Format|models.dat'
    Title = 'Import Old Format'
    Left = 72
    Top = 32
  end
  object PopupMenu1: TPopupMenu
    Left = 488
    Top = 136
    object DeleteModel1: TMenuItem
      Caption = 'Delete Model(s)'
      OnClick = DeleteModel1Click
    end
    object RecreateModel1: TMenuItem
      Caption = 'Recreate Model'
      OnClick = RecreateModel1Click
    end
    object ApplyFlags1: TMenuItem
      Caption = 'Apply Flags'
      object SetTileable1: TMenuItem
        Caption = 'Set "Tileable" Flag'
        OnClick = SetTileable1Click
      end
      object UnsetTileableflag1: TMenuItem
        Caption = 'Unset "Tileable" Flag'
        OnClick = SetTileable1Click
      end
      object SetCastsShadowFlag1: TMenuItem
        Caption = 'Set "Casts Shadow" Flag'
        OnClick = SetTileable1Click
      end
      object UnsetCastsShadowFlag1: TMenuItem
        Caption = 'Unset "Casts Shadow" Flag'
        OnClick = SetTileable1Click
      end
    end
    object Import3DS1: TMenuItem
      Caption = 'Import 3DS...'
      OnClick = Import3DS1Click
    end
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'uim'
    FileName = '*.uim'
    Filter = 'Ultima Iris Game Models|*.uim'
    Left = 112
    Top = 32
  end
  object OpenDialog3DS: TOpenDialog
    DefaultExt = '3ds'
    Filter = '3DS Files|*.3ds'
    Left = 144
    Top = 32
  end
  object OpenPictureDialog: TOpenPictureDialog
    DefaultExt = '*.jpg'
    Filter = 'JPEGs (*.jpg;*.jpeg)|*.jpg;*.jpeg'
    Title = 'Add Texture'
    Left = 176
    Top = 32
  end
end
