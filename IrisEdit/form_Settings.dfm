object SettingsForm: TSettingsForm
  Left = 448
  Top = 129
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Settings'
  ClientHeight = 106
  ClientWidth = 386
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 8
    Top = 8
    Width = 60
    Height = 13
    Caption = 'Tiledata.mul:'
  end
  object Label3: TLabel
    Left = 8
    Top = 32
    Width = 32
    Height = 13
    Caption = 'Art.mul'
  end
  object Label4: TLabel
    Left = 8
    Top = 56
    Width = 45
    Height = 13
    Caption = 'Artidx.mul'
  end
  object Edit2: TEdit
    Left = 72
    Top = 4
    Width = 257
    Height = 21
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
    Left = 72
    Top = 28
    Width = 257
    Height = 21
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
    Left = 72
    Top = 52
    Width = 257
    Height = 21
    TabOrder = 5
  end
  object Button5: TButton
    Left = 224
    Top = 80
    Width = 75
    Height = 22
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 6
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 304
    Top = 80
    Width = 75
    Height = 22
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 7
  end
  object OpenDialog: TOpenDialog
    DefaultExt = '*.mul'
    FileName = 'tiledata.mul'
    InitialDir = '.'
    Left = 72
    Top = 72
  end
end
