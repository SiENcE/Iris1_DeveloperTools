object MainForm: TMainForm
  Left = 222
  Top = 168
  Width = 643
  Height = 498
  Caption = 'Iris - Particle Editor'
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 500
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDefault
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ListView1: TListView
    Left = 0
    Top = 0
    Width = 635
    Height = 452
    Align = alClient
    Columns = <
      item
        Caption = 'Effect Name'
        Width = 154
      end
      item
        AutoSize = True
        Caption = 'Description'
      end
      item
        Caption = 'Texture'
        Width = 120
      end
      item
        Caption = 'Appear Type'
        Width = 100
      end>
    HideSelection = False
    ReadOnly = True
    RowSelect = True
    PopupMenu = PopupMenu1
    TabOrder = 0
    ViewStyle = vsReport
    OnDblClick = ListView1DblClick
  end
  object MainMenu1: TMainMenu
    Left = 120
    Top = 48
    object Particles1: TMenuItem
      Caption = 'Collection'
      object LoadCollection: TMenuItem
        Caption = 'Load Collection'
        OnClick = LoadCollectionClick
      end
      object AddCollection1: TMenuItem
        Caption = 'Add Collection'
      end
      object SaveCollectionas1: TMenuItem
        Caption = 'Save Collection'
        OnClick = SaveCollectionas1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
    object Effects1: TMenuItem
      Caption = 'Effects'
      object AddEffect1: TMenuItem
        Caption = 'Add Effect'
        OnClick = AddEffect1Click
      end
      object LoadEffect1: TMenuItem
        Caption = 'Load Effect'
        OnClick = LoadEffect1Click
      end
    end
    object extures1: TMenuItem
      Caption = 'Textures'
      object SetTextureDir1: TMenuItem
        Caption = 'Set Texture Dir'
        OnClick = SetTextureDir1Click
      end
    end
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'xml'
    Filter = 'Particle Collections|*.xml'
    InitialDir = '.'
    Left = 56
    Top = 16
  end
  object OpenDialog: TOpenDialog
    DefaultExt = 'xml'
    Filter = 'Particle Collections|*.xml'
    InitialDir = '.'
    Left = 88
    Top = 16
  end
  object OpenEffectDialog: TOpenDialog
    DefaultExt = 'xml'
    Filter = 'Effect Definition|*.xml'
    InitialDir = '.'
    Left = 88
    Top = 48
  end
  object PopupMenu1: TPopupMenu
    Left = 232
    Top = 160
    object Add2: TMenuItem
      Caption = 'Add'
      OnClick = AddEffect1Click
    end
    object Load1: TMenuItem
      Caption = 'Load'
      OnClick = LoadEffect1Click
    end
    object Remove1: TMenuItem
      Caption = 'Remove'
      OnClick = Remove1Click
    end
  end
end
