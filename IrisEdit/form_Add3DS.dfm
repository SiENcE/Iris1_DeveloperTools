object Import3DSForm: TImport3DSForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Import 3DS-File'
  ClientHeight = 117
  ClientWidth = 424
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 20
    Height = 13
    Caption = 'File:'
  end
  object Label2: TLabel
    Left = 8
    Top = 32
    Width = 42
    Height = 13
    Caption = 'Texture:'
  end
  object Edit1: TEdit
    Left = 64
    Top = 4
    Width = 273
    Height = 21
    TabOrder = 0
  end
  object Button1: TButton
    Left = 344
    Top = 4
    Width = 75
    Height = 21
    Caption = 'Browse'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Edit2: TEdit
    Left = 64
    Top = 28
    Width = 273
    Height = 21
    TabOrder = 2
  end
  object Button2: TButton
    Left = 344
    Top = 28
    Width = 75
    Height = 21
    Caption = 'Browse'
    TabOrder = 3
    OnClick = Button2Click
  end
  object CheckBox1: TCheckBox
    Left = 224
    Top = 56
    Width = 185
    Height = 17
    Caption = 'Transform to local coord system'
    Checked = True
    State = cbChecked
    TabOrder = 4
  end
  object RadioButton1: TRadioButton
    Left = 8
    Top = 56
    Width = 169
    Height = 17
    Caption = 'Default scale (1 Tile = 20 Units)'
    Checked = True
    TabOrder = 5
    TabStop = True
  end
  object RadioButton2: TRadioButton
    Left = 8
    Top = 72
    Width = 73
    Height = 17
    Caption = 'AutoScale'
    TabOrder = 6
  end
  object RadioButton3: TRadioButton
    Left = 8
    Top = 88
    Width = 97
    Height = 17
    Caption = 'Custom Units:'
    TabOrder = 7
  end
  object Edit3: TEdit
    Left = 112
    Top = 86
    Width = 41
    Height = 21
    TabOrder = 8
    Text = '40'
  end
  object Button3: TButton
    Left = 184
    Top = 80
    Width = 115
    Height = 25
    Caption = 'OK'
    Enabled = False
    TabOrder = 9
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 304
    Top = 80
    Width = 115
    Height = 25
    Caption = 'Cancel'
    TabOrder = 10
    OnClick = Button4Click
  end
  object OpenDialog3DS: TOpenDialog
    DefaultExt = '3ds'
    Filter = '3DS Files|*.3ds'
    Left = 200
    Top = 24
  end
  object OpenPictureDialog: TOpenPictureDialog
    DefaultExt = '*.jpg'
    Filter = 'JPEGs (*.jpg;*.jpeg)|*.jpg;*.jpeg'
    Title = 'Add Texture'
    Left = 168
    Top = 24
  end
end
