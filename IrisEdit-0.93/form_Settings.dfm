object SettingsForm: TSettingsForm
  Left = 448
  Top = 129
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Settings'
  ClientHeight = 164
  ClientWidth = 383
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Label2: TLabel
    Left = 8
    Top = 8
    Width = 59
    Height = 14
    Caption = 'Tiledata.mul:'
  end
  object Label3: TLabel
    Left = 8
    Top = 32
    Width = 34
    Height = 14
    Caption = 'Art.mul'
  end
  object Label4: TLabel
    Left = 8
    Top = 56
    Width = 48
    Height = 14
    Caption = 'Artidx.mul'
  end
  object Label1: TLabel
    Left = 8
    Top = 80
    Width = 59
    Height = 14
    Caption = 'Color Depth:'
  end
  object Label5: TLabel
    Left = 168
    Top = 80
    Width = 76
    Height = 14
    Caption = 'Z-Buffer Depth:'
  end
  object Edit2: TEdit
    Left = 80
    Top = 4
    Width = 249
    Height = 22
    TabOrder = 0
  end
  object Button2: TButton
    Left = 335
    Top = 4
    Width = 43
    Height = 22
    Caption = '...'
    TabOrder = 1
    OnClick = Button3Click
  end
  object Edit3: TEdit
    Left = 80
    Top = 28
    Width = 249
    Height = 22
    TabOrder = 2
  end
  object Button3: TButton
    Left = 335
    Top = 28
    Width = 43
    Height = 22
    Caption = '...'
    TabOrder = 3
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 335
    Top = 52
    Width = 43
    Height = 22
    Caption = '...'
    TabOrder = 4
    OnClick = Button3Click
  end
  object Edit4: TEdit
    Left = 80
    Top = 52
    Width = 249
    Height = 22
    TabOrder = 5
  end
  object Button5: TButton
    Left = 224
    Top = 104
    Width = 75
    Height = 22
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 6
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 304
    Top = 104
    Width = 75
    Height = 22
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 7
  end
  object ComboBox1: TComboBox
    Left = 80
    Top = 76
    Width = 81
    Height = 22
    Style = csDropDownList
    ItemHeight = 14
    TabOrder = 8
    Items.Strings = (
      '16-bit'
      '24-bit'
      '32-bit')
  end
  object ComboBox2: TComboBox
    Left = 248
    Top = 76
    Width = 81
    Height = 22
    Style = csDropDownList
    ItemHeight = 14
    TabOrder = 9
    Items.Strings = (
      '16-bit'
      '24-bit'
      '32-bit')
  end
  object OpenDialog: TOpenDialog
    DefaultExt = '*.mul'
    FileName = 'tiledata.mul'
    InitialDir = '.'
    Left = 72
    Top = 104
  end
end
