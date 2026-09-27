using System;
using System.IO.Ports;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        private SerialPort serialPort;
        private bool alertaMostrada = false;
        private int contadorMuestras = 0;
        private int numeroBotellas = 0;
        private int estado = 8; // Estado inicial: Finalizado o detenido.

        public Form1()
        {
            InitializeComponent();
            ConfigurarPuertoSerial();
            InicializarGraficas();
            lblEstado.Text = "Estado: Detenido"; // Inicialización del estado
        }

        private void ConfigurarPuertoSerial()
        {
            try
            {
                serialPort = new SerialPort("COM8", 9600, Parity.None, 8, StopBits.One);
                serialPort.DataReceived += SerialPort_DataReceived;
                serialPort.Open();
                MessageBox.Show("Puerto serial configurado correctamente.");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error al abrir el puerto serial: {ex.Message}");
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string data = serialPort.ReadLine().Trim();

            if (this.IsHandleCreated)
            {
                this.Invoke(new Action(() =>
                {
                    string[] valores = data.Split(':');
                    if (valores.Length == 4 &&
                        int.TryParse(valores[0], out int botellaActual) &&
                        double.TryParse(valores[1], out double porcentaje) &&
                        double.TryParse(valores[2], out double volumen) &&
                        int.TryParse(valores[3], out int estado))
                    {
                        ActualizarGraficas(botellaActual, porcentaje, volumen, estado);
                    }
                }));
            }
        }

        private void ActualizarGraficas(int botellaActual, double porcentaje, double volumen, int estado)
        {
            // Ajustar valores fuera de rango
            if (porcentaje > 100) porcentaje = 100;
            if (porcentaje < 0) porcentaje = 0;
            if (volumen > 10) volumen = 10;
            if (volumen < 0) volumen = 0;

            // Limitar a 50 muestras
            if (chartPorcentaje.Series[0].Points.Count >= 50)
            {
                chartPorcentaje.Series[0].Points.RemoveAt(0);
                chartVolumen.Series[0].Points.RemoveAt(0);
            }

            chartPorcentaje.Series[0].Points.AddY(porcentaje);
            chartVolumen.Series[0].Points.AddY(volumen);

            lblEstado.Text = $"Estado: {(estado == 7 ? "Iniciando" : estado == 8 ? "Finalizado" : "Emergencia")}";
            lblPorcentaje.Text = $"Porcentaje Actual: {porcentaje:F2}%";
            lblVolumen.Text = $"Volumen Actual: {volumen:F2} L";
            lblBotellas.Text = $"Botellas Llenas: {botellaActual} / {numeroBotellas}";

            if (porcentaje < 50 && !alertaMostrada)
            {
                MessageBox.Show("¡El nivel del tanque está por debajo del 50%! Requiere llenado.", "Alerta", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                alertaMostrada = true;
            }
            else if (porcentaje >= 50)
            {
                alertaMostrada = false;
            }
        }

        private void btnEnviarBotellas_Click(object sender, EventArgs e)
        {
            if (int.TryParse(txtBotellas.Text.Trim(), out int botellas) && botellas > 0 && botellas <= 6)
            {
                numeroBotellas = botellas;
                serialPort.WriteLine(botellas.ToString());
                MessageBox.Show($"Número de botellas enviado: {botellas}");
            }
            else
            {
                MessageBox.Show("Por favor, ingrese un número válido entre 1 y 6.");
            }
        }

        private void btnIniciar_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine("7");
            MessageBox.Show("Proceso iniciado.");
        }

        private void btnParar_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine("8");
            MessageBox.Show("Proceso detenido.");
        }

        private void btnEmergencia_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine("9");
            MessageBox.Show("Paro de emergencia activado.");
        }

        private void InicializarGraficas()
        {
            ConfigurarSerie(chartPorcentaje, "Porcentaje", "Porcentaje (%)", System.Drawing.Color.Blue);
            ConfigurarSerie(chartVolumen, "Volumen", "Volumen (L)", System.Drawing.Color.Green);
        }

        private void ConfigurarSerie(Chart chart, string nombre, string tituloY, System.Drawing.Color color)
        {
            chart.Series.Clear();
            chart.ChartAreas.Clear();
            chart.ChartAreas.Add("Area");

            var serie = chart.Series.Add(nombre);
            serie.ChartType = SeriesChartType.Spline;
            serie.BorderWidth = 3;
            serie.Color = color;

            chart.ChartAreas["Area"].AxisX.Title = "Muestras";
            chart.ChartAreas["Area"].AxisY.Title = tituloY;
            chart.ChartAreas["Area"].AxisY.Minimum = 0;
            chart.ChartAreas["Area"].AxisY.Maximum = 100;
        }
    }
}
