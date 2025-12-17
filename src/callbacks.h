#include <gtk/gtk.h>
#include "fonctions.h"
extern int sexe_ajout;  // 0 = Femme, 1 = Homme

void on_femme_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_homme_toggled(GtkToggleButton *togglebutton, gpointer user_data);

void
on_ajouter_clicked                     (GtkWidget       *objet_graphique,
                                        gpointer         user_data);


void
on_annuler_clicked                     (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_chercher_clicked                    (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_modifier_clicked                    (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_annulern_clicked                    (GtkWidget       *objet_graphique,
                                        gpointer         user_data);


void
on_recherche_clicked                   (GtkWidget       *objet_graphique,
                                        gpointer         user_data);
void
on_cherche_clicked                    (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_treeview4_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_afficher_clicked                    (GtkWidget       *objet_graphique,
                                        gpointer         user_data);


void
on_reserver_clicked                    (GtkWidget       *objet_graphique,
                                        gpointer         user_data);



void
on_voir_clicked                        (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_retour_clicked                      (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_treeview5_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_voir_clicked                        (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_affiche_clicked                     (GtkWidget       *objet_graphique,
                                        gpointer         user_data);
void initialiser_treeview_reservation(GtkWidget *treeview5);

void
on_valider_clicked                     (GtkWidget       *objet_graphique,
                                        gpointer         user_data);
void
on_demande_clicked                     (GtkWidget       *objet_graphique,
                                        gpointer         user_data);
void
on_membre_clicked                      (GtkWidget       *objet_graphique,
                                        gpointer         user_data);
const char* get_utilisateur_connecte_id(void);
const char* get_utilisateur_type(void);
int est_connecte(void);
int verifier_admin(const char *id, const char *mdp);
int verifier_membre(const char *id, const char *mdp);
void initialiser_treeview_reservation_editable(GtkWidget *treeview5);
void on_objectif_toggled(GtkCellRendererToggle *cell, gchar *path_str, gpointer user_data);
void on_jour_toggled(GtkCellRendererToggle *cell, gchar *path_str, gpointer user_data);
void on_disponibilite_edited(GtkCellRendererCombo *cell, gchar *path_str, gchar *new_text, gpointer user_data);

void
on_home_clicked                        (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_home2_clicked                       (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_home3_clicked                       (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_home4_clicked                       (GtkWidget       *objet_graphique,
                                        gpointer         user_data);

void
on_statistique_clicked                 (GtkWidget       *objet_graphique,
                                        gpointer         user_data);
void on_generer_rapport_clicked(GtkWidget *objet_graphique, gpointer user_data);

void
on_eye_auth_clicked                    (GtkWidget       *button,
                                        gpointer         user_data);

void
on_histogramme_clicked                 (GtkWidget       *objet_graphique,
                                        gpointer         user_data);
gboolean on_draw_histogramme(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);
void on_export_histogramme_clicked(GtkWidget *button, gpointer user_data);
void on_histogramme_window_destroy(GtkWidget *window, gpointer user_data);




void
on_radiobutton46_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton47_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);


//centre

/* =================== FONCTIONS POUR L'AJOUT =================== */
void on_radiobuttonmixte_ajout_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiomasculin_ajout_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiofeminin_ajout_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_buttonajouter_clicked(GtkWidget *button, gpointer user_data);
void on_buttonannuler_ajout_clicked(GtkWidget *button, gpointer user_data);
void on_buttonquitter_ajout_clicked(GtkWidget *button, gpointer user_data);

/* =================== FONCTIONS POUR LA MODIFICATION =================== */
void on_radiomixte_modif_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiomasculin_modif_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiofeminin_modif_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_buttonrechercher_modif_clicked(GtkWidget *button, gpointer user_data);
void on_buttonmodifier_clicked(GtkWidget *button, gpointer user_data);
void on_buttonannuler_modif_clicked(GtkWidget *button, gpointer user_data);
void on_buttonquitter_modif_clicked(GtkWidget *button, gpointer user_data);

/* =================== FONCTIONS POUR LA SUPPRESSION =================== */
void on_buttonrechercher_supp_clicked(GtkWidget *button, gpointer user_data);
void on_buttonsupprimer_clicked(GtkWidget *button, gpointer user_data);
void on_buttonannuler_supp_clicked(GtkWidget *button, gpointer user_data);
void on_buttonquitter_supp_clicked(GtkWidget *button, gpointer user_data);

/* =================== FONCTIONS POUR LA RECHERCHE =================== */
void on_buttonrechercher_clicked(GtkWidget *button, gpointer user_data);
void on_buttonafficher_clicked(GtkWidget *button, gpointer user_data);

/* =================== FONCTIONS UTILITAIRES =================== */
void afficher_message(GtkWidget *parent, gchar *message, gchar *type);
void vider_treeview(GtkWidget *treeview);
void charger_centre_dans_treeview(GtkWidget *treeview, centre c);
centre* lireTousCentres(const char *filename, int *count);


void afficher_tous_centres_au_demarrage(GtkWidget *treeview);
void
on_button_afficher_tous_clicked        (GtkWidget      *button,
                                        gpointer         user_data);


void charger_inscriptions_dans_treeview(GtkWidget *treeview);
void vider_treeview(GtkWidget *treeview);
void
on_buttoninscription_clicked           (GtkWidget      *button,
                                        gpointer         user_data);

void
on_buttonvoirinscription_clicked       (GtkWidget       *button,
                                        gpointer         user_data);

void
on_buttonretour_clicked                (GtkWidget       *button,
                                        gpointer         user_data);

void
on_buttonrefrech_clicked               (GtkWidget       *button,
                                        gpointer         user_data);

void
on_centre_clicked                      (GtkWidget       *objet_graphique,
                                        gpointer         user_data);
