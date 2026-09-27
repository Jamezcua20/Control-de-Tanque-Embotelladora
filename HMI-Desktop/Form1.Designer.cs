namespace WinFormsApp1
{
    partial class Form1
    {
        private System.ComponentModel.IContainer components = null;

        // Componentes de la interfaz
        private System.Windows.Forms.Button btnEnviarBotellas;
        private System.Windows.Forms.Button btnIniciar;
        private System.Windows.Forms.Button btnParar;
        private System.Windows.Forms.Button btnEmergencia;
        private System.Windows.Forms.TextBox txtBotellas;
        private System.Windows.Forms.Label lblEstado;
        private System.Windows.Forms.Label lblPorcentaje;
        private System.Windows.Forms.Label lblVolumen;
        private System.Windows.Forms.Label lblBotellas;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartPorcentaje;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartVolumen;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        private void InitializeComponent()
        {
            this.btnEnviarBotellas = new System.Windows.Forms.Button();
            this.btnIniciar = new System.Windows.Forms.Button();
            this.btnParar = new System.Windows.Forms.Button();
            this.btnEmergencia = new System.Windows.Forms.Button();
            this.txtBotellas = new System.Windows.Forms.TextBox();
            this.lblEstado = new System.Windows.Forms.Label();
            this.lblPorcentaje = new System.Windows.Forms.Label();
            this.lblVolumen = new System.Windows.Forms.Label();
            this.lblBotellas = new System.Windows.Forms.Label();
            this.chartPorcentaje = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chartVolumen = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.chartPorcentaje)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartVolumen)).BeginInit();
            this.SuspendLayout();

            // 
            // btnEnviarBotellas
            // 
            this.btnEnviarBotellas.Location = new System.Drawing.Point(30, 30);
            this.btnEnviarBotellas.Name = "btnEnviarBotellas";
            this.btnEnviarBotellas.Size = new System.Drawing.Size(120, 40);
            this.btnEnviarBotellas.TabIndex = 0;
            this.btnEnviarBotellas.Text = "Enviar Botellas";
            this.btnEnviarBotellas.UseVisualStyleBackColor = true;
            this.btnEnviarBotellas.Click += new System.EventHandler(this.btnEnviarBotellas_Click);
            // 
            // btnIniciar
            // 
            this.btnIniciar.Location = new System.Drawing.Point(160, 30);
            this.btnIniciar.Name = "btnIniciar";
            this.btnIniciar.Size = new System.Drawing.Size(120, 40);
            this.btnIniciar.TabIndex = 1;
            this.btnIniciar.Text = "Iniciar";
            this.btnIniciar.UseVisualStyleBackColor = true;
            this.btnIniciar.Click += new System.EventHandler(this.btnIniciar_Click);
            // 
            // btnParar
            // 
            this.btnParar.Location = new System.Drawing.Point(290, 30);
            this.btnParar.Name = "btnParar";
            this.btnParar.Size = new System.Drawing.Size(120, 40);
            this.btnParar.TabIndex = 2;
            this.btnParar.Text = "Parar";
            this.btnParar.UseVisualStyleBackColor = true;
            this.btnParar.Click += new System.EventHandler(this.btnParar_Click);
            // 
            // btnEmergencia
            // 
            this.btnEmergencia.Location = new System.Drawing.Point(420, 30);
            this.btnEmergencia.Name = "btnEmergencia";
            this.btnEmergencia.Size = new System.Drawing.Size(120, 40);
            this.btnEmergencia.TabIndex = 3;
            this.btnEmergencia.Text = "Emergencia";
            this.btnEmergencia.UseVisualStyleBackColor = true;
            this.btnEmergencia.Click += new System.EventHandler(this.btnEmergencia_Click);
            // 
            // txtBotellas
            // 
            this.txtBotellas.Location = new System.Drawing.Point(30, 80);
            this.txtBotellas.Name = "txtBotellas";
            this.txtBotellas.Size = new System.Drawing.Size(120, 23);
            this.txtBotellas.TabIndex = 4;
            // 
            // lblEstado
            // 
            this.lblEstado.AutoSize = true;
            this.lblEstado.Location = new System.Drawing.Point(30, 120);
            this.lblEstado.Name = "lblEstado";
            this.lblEstado.Size = new System.Drawing.Size(88, 15);
            this.lblEstado.TabIndex = 5;
            this.lblEstado.Text = "Estado: Detenido";
            // 
            // lblPorcentaje
            // 
            this.lblPorcentaje.AutoSize = true;
            this.lblPorcentaje.Location = new System.Drawing.Point(30, 150);
            this.lblPorcentaje.Name = "lblPorcentaje";
            this.lblPorcentaje.Size = new System.Drawing.Size(130, 15);
            this.lblPorcentaje.TabIndex = 6;
            this.lblPorcentaje.Text = "Porcentaje Actual: 0%";
            // 
            // lblVolumen
            // 
            this.lblVolumen.AutoSize = true;
            this.lblVolumen.Location = new System.Drawing.Point(30, 180);
            this.lblVolumen.Name = "lblVolumen";
            this.lblVolumen.Size = new System.Drawing.Size(118, 15);
            this.lblVolumen.TabIndex = 7;
            this.lblVolumen.Text = "Volumen Actual: 0 L";
            // 
            // lblBotellas
            // 
            this.lblBotellas.AutoSize = true;
            this.lblBotellas.Location = new System.Drawing.Point(30, 210);
            this.lblBotellas.Name = "lblBotellas";
            this.lblBotellas.Size = new System.Drawing.Size(136, 15);
            this.lblBotellas.TabIndex = 8;
            this.lblBotellas.Text = "Botellas Llenas: 0 / 0";
            // 
            // chartPorcentaje
            // 
            this.chartPorcentaje.Location = new System.Drawing.Point(30, 240);
            this.chartPorcentaje.Name = "chartPorcentaje";
            this.chartPorcentaje.Size = new System.Drawing.Size(500, 200);
            this.chartPorcentaje.TabIndex = 9;
            // 
            // chartVolumen
            // 
            this.chartVolumen.Location = new System.Drawing.Point(30, 460);
            this.chartVolumen.Name = "chartVolumen";
            this.chartVolumen.Size = new System.Drawing.Size(500, 200);
            this.chartVolumen.TabIndex = 10;

            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(600, 700);
            this.Controls.Add(this.chartVolumen);
            this.Controls.Add(this.chartPorcentaje);
            this.Controls.Add(this.lblBotellas);
            this.Controls.Add(this.lblVolumen);
            this.Controls.Add(this.lblPorcentaje);
            this.Controls.Add(this.lblEstado);
            this.Controls.Add(this.txtBotellas);
            this.Controls.Add(this.btnEmergencia);
            this.Controls.Add(this.btnParar);
            this.Controls.Add(this.btnIniciar);
            this.Controls.Add(this.btnEnviarBotellas);
            this.Name = "Form1";
            this.Text = "Control Embotelladora";
            ((System.ComponentModel.ISupportInitialize)(this.chartPorcentaje)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartVolumen)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        #endregion
    }
}
