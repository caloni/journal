///////////////////////////////////////////////////////////////////////////////
//
//  PlayerSelector.xaml.cs
//
// 
// © 2008 Microsoft Corporation. All Rights Reserved.
//
// This file is licensed as part of the Silverlight 1.1 SDK, for details look here: http://go.microsoft.com/fwlink/?LinkID=111970&clcid=0x409
//
///////////////////////////////////////////////////////////////////////////////
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Collections.Generic;
using System.Windows.Controls.Primitives;

namespace wp7chess
{
	public partial class PlayerSelector : UserControl
	{
        private int selectedPlayer;
		private bool isUnchecking = false;

		public event EventHandler SelectionChanged;

		public PlayerSelector()
		{
			// Required to initialize variables
			InitializeComponent();

			string[] players = this.Players;
			for (int i = 0; i < players.Length; i++)
			{
				ColumnDefinition column = new ColumnDefinition();
				column.Width = new GridLength(1.0 / players.Length, GridUnitType.Star);
				this.LayoutRoot.ColumnDefinitions.Add(column);

                ToggleButton toggleButton = new ToggleButton();
				Grid.SetColumn(toggleButton, i);
				toggleButton.Content = players[i];
				toggleButton.Checked += this.OnToggleButtonChecked;
				toggleButton.Unchecked += this.OnToggleButtonUnchecked;
				this.LayoutRoot.Children.Add(toggleButton);
			}
		}

		public string[] Players
		{
			get { return new string[] { "Human", "C#", "JS" }; }
		}

		public int SelectedPlayer
		{
			get { return this.selectedPlayer; }
			set
			{
				this.selectedPlayer = value;
				((ToggleButton)this.LayoutRoot.Children[value]).IsChecked = true;
			}
		}

		private void OnToggleButtonChecked(object sender, RoutedEventArgs e)
		{
			this.isUnchecking = true;
			foreach (ToggleButton button in this.LayoutRoot.Children)
			{
				if (button == sender)
				{
					this.selectedPlayer = this.LayoutRoot.Children.IndexOf(button);
					if (this.SelectionChanged != null)
					{
						this.SelectionChanged(this, EventArgs.Empty);
					}
				}
				else
				{
					button.IsChecked = false;
				}
			}
			this.isUnchecking = false;
		}

        public void setPlayer(int i)
        {
            this.selectedPlayer = i;
            if (this.SelectionChanged != null)
            {
                this.SelectionChanged(this, EventArgs.Empty);
            }
        }

		private void OnToggleButtonUnchecked(object sender, RoutedEventArgs e)
		{
			if (!this.isUnchecking)
				((ToggleButton)sender).IsChecked = true;
		}
	}
}