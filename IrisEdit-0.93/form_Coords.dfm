object CoordForm: TCoordForm
  Left = 411
  Top = 234
  BorderIcons = []
  BorderStyle = bsNone
  ClientHeight = 21
  ClientWidth = 233
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDeactivate = FormDeactivate
  PixelsPerInch = 96
  TextHeight = 14
  object Edit1: TEdit
    Left = 0
    Top = 0
    Width = 49
    Height = 22
    TabOrder = 0
    Text = '0.0'
    OnKeyPress = Edit1KeyPress
  end
  object Edit2: TEdit
    Left = 48
    Top = 0
    Width = 49
    Height = 22
    TabOrder = 1
    Text = '0.0'
  end
  object Edit3: TEdit
    Left = 96
    Top = 0
    Width = 49
    Height = 22
    TabOrder = 2
    Text = '0.0'
  end
  object Button1: TButton
    Left = 144
    Top = 0
    Width = 41
    Height = 21
    Caption = 'Add'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 184
    Top = 0
    Width = 49
    Height = 21
    Caption = 'Cancel'
    TabOrder = 4
    OnClick = FormDeactivate
  end
end
