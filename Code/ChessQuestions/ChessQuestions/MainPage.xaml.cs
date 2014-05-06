using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using System.Windows.Threading;
using Microsoft.Phone.Shell;

namespace ChessQuestions
{
    public partial class MainPage : PhoneApplicationPage
    {
        //private string player_one = "";
        //private string player_two = "";

        private Position position = new Position();
        private Searcher searcher;
        private DispatcherTimer timer = new DispatcherTimer();
        private bool isGameOver = false;
        static private string[] questions = new string[] {
            "1. DEPOIS DO LANCE DO TEU AMIGO.",
            "2. Foi esse o lance que esperavas?",
            "3. Tinhas previsto isso?",
            "4. Sangue frio vale uma peça!",
            "5. Que peças estão atacadas pelo lance?",
            "6. Como estão defendidas? Estão mesmo?",
            "7. Quantas defesas contra quantos ataques?",
            "8. Examina teu Rei: Está em perigo de xeque? De xeque descoberto? Quantos lugares tem para ir?",
            "9. Que estão fazendo as peças atacadas?",
            "10. Cobrindo alguma coisa?",
            "11. As casas para onde podem fugir estão inteiramente defendidas?",
            "12. Poderiam ser levadas a um lugar de onde atacassem algum ponto vital?",
            //"13. Se está perdida a peça, não seria possível defendê-la por outra peça que, depois da troca, fosse ocupar uma boa posição?",
            "13. Se está perdida a peça, não seria possível defendê-la para troca, ocupando boa posição?",
            "14. Quantos meios há de defendê-la? POUPAI a Dama!",
            "15. Que fazia o lance, onde estava antes?",
            "16. Cobrindo alguma coisa?",
            "17. Que está fazendo agora?",
            "18. O lance está atacado? Não será cilada?",
            "19. Sob a defesa de que peças está o lance, na casa que veio ocupar?",
            "20. O lance descobriu alguma peça que agora também ataca?",
            "21. Ou libertou alguma peça?",
            "22. Está obstruindo alguma peça?",
            "23. Ele ali permitirá que outra peça venha atacar?",
            "24. Estará defendendo uma casa para onde esteja sendo premeditado outro lance?",
            "25. Ou terá desocupado a casa, para que outra peça a venha ocupar?",
            "26. Terá ido ali como defesa \"extra\", que permita soltar uma das defesas atuais?",
            "27. Que \"artes\" anda a Dama inimiga imaginando? Ela está de branco ou de preto?",
            "28. Onde podem os Reis ir, neste momento? Não os perca de vista!...",
            "29. Existe alguma casa comandada por mais de duas ou de três peças inimigas?",
            "30. Que tens para temer?",
            "31. Não poderá ser ainda mais temível?",
            "32. Que tal um ataque indireto?",
            "33. Sangue frio vale uma peça!",
            "34. Por que motivo teria teu amigo feito esse lance?",
            "35. Qual foi mesmo o lance que ele fez antes desse?",
            "36. Qual a posição do jogo do teu amigo?",
            "37. Quais são os pontos fracos? Os pontos fortes? Decide, mas ESPERA!",
            //"Está atacando? O que? Sozinha? Está defendendo? O que? Sozinha? Está atacada? Por que peça? Pode ser atacada? Está defendida? Onde pode ir? Que pode fazer?",
            "Está atacando? Está defendendo? Está atacada? Está defendida? Onde pode ir? Que pode fazer?",
            "38. ANTES DO TEU LANCE.",
            "39. Que estás fazendo, onde se acha a peça que pretendes mover?",
            "40. Podes, realmente, movê-la?",
            "41. Não será que precisas dela, onde ela está?",
            "42. Está ocupando agora uma boa posição?",
            "43. Estará acaso melhor, depois de mover?",
            "44. Descobrirá outra peça que ataca?",
            "45. Irá impedir o avanço de alguma outra peça?",
            "46. Vai ficar atacada, ou passível de ataque, na nova casa para onde pretendes levá-la?",
            "47. Como estará defendida lá?",
            "48. Se ela tiver de recuar, poderá faze-lo com facilidade? Com proveito? Sem vexame?",
            "49. Poderá o teu lance impedir o avanço do teu amigo, ou frustrar os seus planos?",
            "50. Que lance estará ele esperando?",
            "51. Não poderia ele responder-te com um lance mais intenso em outro lugar?",
            "52. As casas para onde pode fugir a peça que pretendes atacar acham-se fortemente defendidas?",
            "53. Qual é a finalidade exata do lance que queres fazer?",
            "54. Não haveria outro meio melhor de fazeres a mesma coisa?",
            "55. De que maneira esse lance servirá a teu Rei?",
            "56. O mais humilde peão merece a mais encarniçada defesa!",
            "57. Existe alguma coisa mais prudente? Então, FOGO!",
            "58. DEPOIS DO TEU LANCE.",
            "59. Que peças estão atacadas pelo lance?",
            "60. Como estão defendidas? Estão mesmo?",
            "61. Quantas defesas contra quantos ataques?",
            "62. Examina a posição do Rei inimigo: Está em perigo de xeque?  De xeque descoberto?",
            "63. Que estão fazendo as peças atacadas?",
            "64. Cobrindo alguma coisa?",
            "65. As casas para onde podem fugir estão inteiramente defendidas?",
            "66. Poderiam ser levadas a um lugar de onde atacassem algum ponto vital?",
            "67. Que fazia o lance, onde estava antes?",
            "68. Cobrindo alguma coisa?",
            "69. Que está fazendo agora?",
            "70. Sob a defesa de que peças está o lance, na casa que foi ocupar?",
            "71. O lance descobriu alguma peça que agora também ataca?",
            "72. Ou libertou alguma peça?",
            "73. Está obstruindo alguma peça?",
            "74. Ele ali permitirá que outra peça vá atacar?",
            "75. Onde pode os Reis, ir, neste Momento? Não os perca de vista!...",
            "76. Qual é a posição do teu jogo?",
            "77. Quais são os pontos fracos? Os pontos fortes?",
            //"Está atacando? O que? Sozinha? Está defendendo? O que? Sozinha? Está atacada? Por que peça? Pode ser atacada? Está defendida? Onde pode ir? Que pode fazer?"
            "Está atacando? Está defendendo? Está atacada? Está defendida? Onde pode ir? Que pode fazer?"
        };
        private int nextQuestion = 1;
        private bool nextClickOff = false;

        ApplicationBarMenuItem human_vs_phone;
        ApplicationBarMenuItem human_vs_human;
        ApplicationBarMenuItem phone_phone;

        public MainPage()
        {
            InitializeComponent();

            Color color = new Color();
            color.A = 0;
            color.B = 0;
            color.G = 0;
            color.R = 0;

            ApplicationBar = new ApplicationBar();
            ApplicationBar.BackgroundColor = color;
            ApplicationBar.IsMenuEnabled = true;
            ApplicationBar.IsVisible = true;
            ApplicationBar.Opacity = 0.8;

            ApplicationBarIconButton refresh = new ApplicationBarIconButton(new Uri("/appbar_refresh.png", UriKind.Relative));
            refresh.Text = "new game";
            refresh.Click += new EventHandler(refresh_Click);

            ApplicationBar.Buttons.Add(refresh);

            human_vs_phone = new ApplicationBarMenuItem("» humano vs telefone");
            human_vs_phone.Click += new EventHandler(human_vs_phone_Click);
            ApplicationBar.MenuItems.Add(human_vs_phone);

            human_vs_human = new ApplicationBarMenuItem("humano versus humano");
            human_vs_human.Click += new EventHandler(human_vs_human_Click);
            ApplicationBar.MenuItems.Add(human_vs_human);

            phone_phone = new ApplicationBarMenuItem("telefone vs telefone");
            phone_phone.Click += new EventHandler(phone_phone_Click);
            ApplicationBar.MenuItems.Add(phone_phone);

            load();
        }

        void blankMenu()
        {
            phone_phone.Text = "telefone vs telefone";
            human_vs_phone.Text = "humano vs telefone";
            human_vs_human.Text = "humano vs humano";
        }

        void phone_phone_Click(object sender, EventArgs e)
        {
            blankMenu();
            phone_phone.Text = "» telefone vs telefone";

            blackSelector.setPlayer(1);
            whiteSelector.setPlayer(1);
        }

        void human_vs_phone_Click(object sender, EventArgs e)
        {
            blankMenu();
            human_vs_phone.Text = "» humano vs telefone";
            blackSelector.setPlayer(1);
            whiteSelector.setPlayer(0);
        }

        void human_vs_human_Click(object sender, EventArgs e)
        {
            blankMenu();
            human_vs_human.Text = "» humano vs humano";
            blackSelector.setPlayer(0);
            whiteSelector.setPlayer(0);
        }

        private void load()
        {
            this.position.ResetBoard();

            this.searcher = new Searcher(this.position);
            this.searcher.AllottedTime = 200;

            this.chessBoard.UserMove += this.OnUserMove;
            this.chessBoard.AnimationCompleted += this.OnChessBoardAnimationCompleted;

            this.whiteSelector.SelectedPlayer = 0;
            this.blackSelector.SelectedPlayer = 1;
            this.whiteSelector.SelectionChanged += this.OnSelectorSelectionChanged;
            this.blackSelector.SelectionChanged += this.OnSelectorSelectionChanged;

            this.timer.Interval = TimeSpan.FromMilliseconds(10);
            this.timer.Tick += this.OnTimerTick;

            this.LayoutUpdated += this.OnRootControlLayoutUpdated;

            this.Loaded += this.OnRootControlLoaded;
        }

        void refresh_Click(object sender, EventArgs e)
        {
            this.NavigationService.Navigate(new Uri("/Refresh.xaml", UriKind.Relative));
        }

        private void OnRootControlLoaded(object sender, RoutedEventArgs e)
        {
            this.chessBoard.Position = this.position;
        }

        private void OnRootControlLayoutUpdated(object sender, EventArgs e)
        {
            this.whiteSelector.Width = this.chessBoard.ActualWidth;
            this.blackSelector.Width = this.chessBoard.ActualWidth;
        }

        private void OnUserMove(ushort move)
        {
            this.position.MakeMove(move);
            this.chessBoard.UpdateForNewPosition();
        }

        private void OnChessBoardAnimationCompleted()
        {
            // ushort moves = HtmlPage.Window.Invoke("GenerateValidMoves");


            if (this.position.GenerateValidMoves().Count == 0)
            // if (this.jscriptPlayer.ValidMoves().Count == 0)
            {
                if (this.isGameOver)
                {
                    return;
                }

                this.isGameOver = true;
                // Game over state
                //WinnerDisplayControl winner = new WinnerDisplayControl();
                //winner.Victor = this.position.ToMove == 0 ? "White" : "Black";
                //winner.Completed += this.OnWinnerCompleted;
                //winner.Width = 400;
                //winner.Height = 250;
                //this.LayoutRoot.Children.Add(winner);

                string winnerColor = this.position.ToMove == 0 ? "As brancas" : "As pretas";
                MessageBox.Show(winnerColor + " ganharam!");

            }
            else
            {
                this.DoAITurn();
            }
        }

        private void OnWinnerCompleted(object sender, RoutedEventArgs e)
        {
            this.isGameOver = false;
            this.LayoutRoot.Children.Remove((UIElement)sender);

            this.position.ResetBoard();
            this.chessBoard.UpdateForNewPosition();
        }

        private void OnSelectorSelectionChanged(object sender, EventArgs e)
        {
            this.DoAITurn();
        }

        private void OnTimerTick(object sender, EventArgs e)
        {
            this.timer.Stop();
            this.chessBoard.UpdateForNewPosition();
        }

        private void DoAITurn()
        {
            int color = this.position.ToMove;
            PlayerSelector selector = color == 0 ? this.blackSelector : this.whiteSelector;

            if (selector.SelectedPlayer == 0)
            {
                this.UpdateNodesSec(color, "Human", 3, 1000);
                return;
            }

            if (selector.SelectedPlayer == 1)
            {
                this.position.MakeMove(this.searcher.Search());
                this.UpdateNodesSec(color, "C#", this.searcher.QNodeCount + this.searcher.NodeCount, this.searcher.AllottedTime);
            }
            else if (selector.SelectedPlayer == 2)
            {
                //this.jscriptPlayer.Fen = this.position.GetFen();
                //this.position.MakeMove(this.jscriptPlayer.MakeMove());
                //this.UpdateNodesSec(color, "JS", this.jscriptPlayer.Nodes, this.jscriptPlayer.Time);

            }

            this.timer.Start();
        }

        private void UpdateNodesSec(int color, string aiName, int nodes, int time)
        {
            long nodesSec = ((long)nodes * 1000) / time;
            if (color == 0)
            {
                this.blackNodesSec.Text = nodesSec.ToString();
                this.blackNodesTitle.Text = aiName + " Nodes/Sec.";
            }
            else
            {
                this.whiteNodesSec.Text = nodesSec.ToString();
                this.whiteNodesTitle.Text = aiName + " Nodes/Sec.";
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            nextQuestion = 0;
            Question.Text = questions[nextQuestion++];
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            if (nextClickOff == false)
            {
                if (nextQuestion + 1 > questions.Length)
                    nextQuestion = 0;
                Question.Text = questions[nextQuestion++];
            }
            else nextClickOff = false;
        }

        private void Button_Hold_1(object sender, GestureEventArgs e)
        {
            nextQuestion = 0;
            Question.Text = questions[nextQuestion++];
            nextClickOff = true;
        }
    }
}