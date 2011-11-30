object ParticleTextureForm: TParticleTextureForm
  Left = 227
  Top = 154
  BorderStyle = bsSingle
  Caption = 'Particle Types'
  ClientHeight = 330
  ClientWidth = 577
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 296
    Top = 8
    Width = 273
    Height = 281
    Caption = 'Texture'
    TabOrder = 5
    object Image1: TImage
      Left = 8
      Top = 16
      Width = 256
      Height = 256
    end
  end
  object Button1: TButton
    Left = 296
    Top = 296
    Width = 161
    Height = 22
    Caption = 'Load Texture'
    TabOrder = 3
    OnClick = Button1Click
  end
  object ListView1: TListView
    Left = 8
    Top = 8
    Width = 281
    Height = 209
    Columns = <
      item
        Caption = 'ID'
        Width = 30
      end
      item
        AutoSize = True
        Caption = 'Description'
      end
      item
        Caption = 'Position'
        Width = 60
      end
      item
        Caption = 'Size'
        Width = 60
      end>
    HideSelection = False
    ReadOnly = True
    RowSelect = True
    TabOrder = 0
    ViewStyle = vsReport
    OnChange = ListView1Change
  end
  object Button2: TButton
    Left = 464
    Top = 296
    Width = 105
    Height = 22
    Caption = 'OK'
    TabOrder = 4
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 96
    Top = 296
    Width = 89
    Height = 22
    Caption = 'Add'
    TabOrder = 1
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 192
    Top = 296
    Width = 97
    Height = 22
    Caption = 'Del'
    TabOrder = 2
    OnClick = Button4Click
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 224
    Width = 281
    Height = 65
    TabOrder = 6
    object Label5: TLabel
      Left = 8
      Top = 16
      Width = 56
      Height = 13
      Caption = 'Description:'
    end
    object Label2: TLabel
      Left = 64
      Top = 40
      Width = 10
      Height = 13
      Caption = 'Y:'
    end
    object Label1: TLabel
      Left = 8
      Top = 40
      Width = 10
      Height = 13
      Caption = 'X:'
    end
    object Label3: TLabel
      Left = 120
      Top = 40
      Width = 31
      Height = 13
      Caption = 'Width:'
    end
    object Label4: TLabel
      Left = 200
      Top = 40
      Width = 31
      Height = 13
      Caption = 'Height'
    end
    object Edit5: TEdit
      Left = 80
      Top = 12
      Width = 193
      Height = 21
      TabOrder = 0
      OnChange = Edit1Change
    end
    object Edit1: TEdit
      Left = 24
      Top = 36
      Width = 33
      Height = 21
      TabOrder = 1
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit2: TEdit
      Left = 80
      Top = 36
      Width = 33
      Height = 21
      TabOrder = 2
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit3: TEdit
      Left = 160
      Top = 36
      Width = 33
      Height = 21
      TabOrder = 3
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
    object Edit4: TEdit
      Left = 240
      Top = 36
      Width = 33
      Height = 21
      TabOrder = 4
      OnChange = Edit1Change
      OnKeyPress = Edit1KeyPress
    end
  end
  object OpenDialog: TOpenDialog
    DefaultExt = 'png'
    Filter = 'PNG Files|*.png'
    InitialDir = '.'
    Left = 48
    Top = 296
  end
end
