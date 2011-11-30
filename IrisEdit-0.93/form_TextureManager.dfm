object TextureManagerForm: TTextureManagerForm
  Left = 41
  Top = 73
  Width = 905
  Height = 623
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Texture Manager'
  Color = clBtnFace
  Constraints.MinHeight = 480
  Constraints.MinWidth = 640
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDefault
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Splitter1: TSplitter
    Left = 529
    Top = 0
    Height = 550
  end
  object ListView: TListView
    Left = 0
    Top = 0
    Width = 529
    Height = 550
    Align = alLeft
    Columns = <
      item
        Caption = 'Name'
        Width = 120
      end
      item
        Caption = 'Size'
        Width = 100
      end
      item
        Caption = 'Use Count'
        Width = 80
      end
      item
        AutoSize = True
        Caption = 'Checksum'
      end>
    ReadOnly = True
    RowSelect = True
    SortType = stData
    TabOrder = 0
    ViewStyle = vsReport
    OnChange = ListViewChange
    OnDblClick = ListViewDblClick
  end
  object Panel1: TPanel
    Left = 532
    Top = 0
    Width = 365
    Height = 550
    Align = alClient
    BevelOuter = bvNone
    BorderStyle = bsSingle
    TabOrder = 1
    object PaintBox: TPaintBox
      Left = 0
      Top = 0
      Width = 361
      Height = 319
      Align = alClient
      OnPaint = PaintBoxPaint
    end
    object GroupBox1: TGroupBox
      Left = 0
      Top = 319
      Width = 361
      Height = 122
      Align = alBottom
      Caption = 'Infos:'
      TabOrder = 0
      object Label1: TLabel
        Left = 8
        Top = 16
        Width = 30
        Height = 14
        Caption = 'Name:'
      end
      object Label2: TLabel
        Left = 8
        Top = 40
        Width = 24
        Height = 14
        Caption = 'Size:'
      end
      object Label3: TLabel
        Left = 8
        Top = 64
        Width = 53
        Height = 14
        Caption = 'Checksum:'
      end
      object Shape1: TShape
        Left = 88
        Top = 92
        Width = 65
        Height = 25
      end
      object Label4: TLabel
        Left = 176
        Top = 40
        Width = 53
        Height = 14
        Caption = 'Use Count:'
      end
      object Edit1: TEdit
        Left = 72
        Top = 12
        Width = 281
        Height = 22
        Color = clBtnFace
        ReadOnly = True
        TabOrder = 0
      end
      object Edit2: TEdit
        Left = 72
        Top = 36
        Width = 97
        Height = 22
        Color = clBtnFace
        ReadOnly = True
        TabOrder = 1
      end
      object Edit3: TEdit
        Left = 72
        Top = 60
        Width = 281
        Height = 22
        Color = clBtnFace
        ReadOnly = True
        TabOrder = 2
      end
      object CheckBox1: TCheckBox
        Left = 8
        Top = 96
        Width = 73
        Height = 17
        Caption = 'Color Key'
        TabOrder = 3
        OnClick = CheckBox1Click
      end
      object Button1: TButton
        Left = 160
        Top = 92
        Width = 49
        Height = 25
        Caption = 'Change'
        TabOrder = 4
        OnClick = Button1Click
      end
      object Edit4: TEdit
        Left = 256
        Top = 36
        Width = 97
        Height = 22
        Color = clBtnFace
        ReadOnly = True
        TabOrder = 5
      end
      object Button4: TButton
        Left = 216
        Top = 92
        Width = 57
        Height = 25
        Caption = 'Export'
        TabOrder = 6
        OnClick = Button4Click
      end
    end
    object GroupBox2: TGroupBox
      Left = 0
      Top = 441
      Width = 361
      Height = 105
      Align = alBottom
      Caption = 'Replace Ground Textures'
      TabOrder = 1
      object PaintBox1: TPaintBox
        Left = 184
        Top = 24
        Width = 161
        Height = 73
        OnPaint = PaintBox1Paint
      end
      object ListBox1: TListBox
        Left = 8
        Top = 24
        Width = 89
        Height = 73
        ItemHeight = 14
        TabOrder = 0
        OnClick = ListBox1Click
      end
      object Edit5: TEdit
        Left = 112
        Top = 24
        Width = 57
        Height = 22
        TabOrder = 1
      end
      object Button2: TButton
        Left = 112
        Top = 48
        Width = 57
        Height = 21
        Caption = 'Add'
        TabOrder = 2
        OnClick = Button2Click
      end
      object Button3: TButton
        Left = 112
        Top = 72
        Width = 57
        Height = 21
        Caption = 'Remove'
        TabOrder = 3
        OnClick = Button3Click
      end
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 550
    Width = 897
    Height = 19
    Panels = <>
    SimplePanel = True
    SimpleText = '131 textures loaded'
  end
  object MainMenu1: TMainMenu
    Left = 408
    Top = 104
    object Texture1: TMenuItem
      Caption = '&Textures'
      object AddTexture1: TMenuItem
        Caption = '&Add Texture...'
        OnClick = AddTexture1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Close1: TMenuItem
        Caption = '&Close'
        OnClick = Close1Click
      end
    end
  end
  object OpenPictureDialog: TOpenPictureDialog
    DefaultExt = '*.jpg'
    Filter = 'JPEGs (*.jpg;*.jpeg)|*.jpg;*.jpeg'
    Title = 'Add Texture'
    Left = 344
    Top = 104
  end
  object ColorDialog: TColorDialog
    Left = 376
    Top = 104
  end
  object SavePictureDialog1: TSavePictureDialog
    Left = 448
    Top = 104
  end
end
