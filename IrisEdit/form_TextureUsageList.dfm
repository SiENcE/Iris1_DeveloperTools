object TextureUsageForm: TTextureUsageForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Texture Usage List'
  ClientHeight = 440
  ClientWidth = 752
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object ListView: TListView
    Left = 0
    Top = 0
    Width = 401
    Height = 433
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
      end
      item
        Caption = 'Usage'
        Width = 70
      end>
    GridLines = True
    HideSelection = False
    MultiSelect = True
    ReadOnly = True
    RowSelect = True
    TabOrder = 0
    ViewStyle = vsReport
    OnChange = ListViewChange
    OnDblClick = Button1Click
  end
  object Button1: TButton
    Left = 408
    Top = 408
    Width = 161
    Height = 25
    Caption = 'Open Model'
    TabOrder = 1
    OnClick = Button1Click
  end
  object GroupBox1: TGroupBox
    Left = 408
    Top = 0
    Width = 337
    Height = 404
    Caption = 'Texture'
    TabOrder = 2
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 31
      Height = 13
      Caption = 'Name:'
    end
    object Label2: TLabel
      Left = 8
      Top = 40
      Width = 23
      Height = 13
      Caption = 'Size:'
    end
    object Label4: TLabel
      Left = 176
      Top = 40
      Width = 66
      Height = 13
      Caption = 'Usage Count:'
    end
    object Label3: TLabel
      Left = 8
      Top = 64
      Width = 52
      Height = 13
      Caption = 'Checksum:'
    end
    object PaintBox: TPaintBox
      Left = 2
      Top = 88
      Width = 333
      Height = 313
      OnPaint = PaintBoxPaint
    end
    object Edit1: TEdit
      Left = 72
      Top = 12
      Width = 249
      Height = 21
      Color = clBtnFace
      ReadOnly = True
      TabOrder = 0
    end
    object Edit2: TEdit
      Left = 72
      Top = 36
      Width = 97
      Height = 21
      Color = clBtnFace
      ReadOnly = True
      TabOrder = 1
    end
    object Edit4: TEdit
      Left = 248
      Top = 36
      Width = 73
      Height = 21
      Color = clBtnFace
      ReadOnly = True
      TabOrder = 2
    end
    object Edit3: TEdit
      Left = 72
      Top = 60
      Width = 249
      Height = 21
      Color = clBtnFace
      ReadOnly = True
      TabOrder = 3
    end
  end
  object Button2: TButton
    Left = 576
    Top = 408
    Width = 169
    Height = 25
    Caption = 'Close'
    TabOrder = 3
    OnClick = Button2Click
  end
end
