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
// entraineurs
void on_button_ajouter_clicked(GtkWidget *button, gpointer user_data);
void on_button_rechercher_clicked(GtkWidget *button, gpointer user_data);
void on_button_modifier_clicked(GtkWidget *button, gpointer user_data);
void on_button_enregistrer_modif_clicked(GtkWidget *button, gpointer user_data);
void on_button_supprimer_clicked(GtkWidget *button, gpointer user_data);
void on_button_afficher_tous_clicked(GtkWidget *button, gpointer user_data);
void on_radiobutton3_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton4_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton1_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton2_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_spinbutton_experience_value_changed(GtkSpinButton *spinbutton, gpointer user_data);
void on_spinbutton_specialite_value_changed(GtkSpinButton *spinbutton, gpointer user_data);
void on_spinbutton_experience2_value_changed(GtkSpinButton *spinbutton, gpointer user_data);
void on_spinbutton_specialite2_value_changed(GtkSpinButton *spinbutton, gpointer user_data);
void on_ajout_button95_clicked(GtkWidget *button, gpointer user_data);
void on_button96_clicked(GtkWidget *button, gpointer user_data);
void on_button97_clicked(GtkWidget *button, gpointer user_data);
void on_spinbutton1_changed(GtkSpinButton *spinbutton, gpointer user_data);
void on_spinbutton2_changed(GtkSpinButton *spinbutton, gpointer user_data);
void on_ajout_dispo_comboboxentry23_changed(GtkComboBox *combobox, gpointer user_data);
void on_comboboxentry25_changed(GtkComboBox *combobox, gpointer user_data);
void on_treeview6_row_activated(GtkTreeView *treeview, GtkTreePath *path,
                                 GtkTreeViewColumn *column, gpointer user_data);

void on_lundi_checkbutton1_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_mardi_checkbutton2_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_mercredi_checkbutton3_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_jeudi_checkbutton4_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_vendredi_checkbutton5_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_samedi_checkbutton6_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_lundi_checkbutton7_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_mardi_checkbutton8_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_mercredi_checkbutton9_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_jeudi_checkbutton10_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_vendredi_checkbutton11_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_samedi_checkbutton12_toggled(GtkToggleButton *togglebutton, gpointer user_data);

void on_rechercher_id_clicked(GtkWidget *button, gpointer user_data);
// Ajoutez ces lignes à la fin de callbacks.h, avant #endif

void on_button98_clicked(GtkWidget *button, gpointer user_data);
void on_treeview6_row_activated(GtkTreeView *treeview, GtkTreePath *path,
                                GtkTreeViewColumn *column, gpointer user_data);
// callbacks.h - Ajouter ces lignes à la fin
void on_button99_clicked(GtkWidget *button, gpointer user_data);
void on_button_valider_client100_clicked(GtkWidget *button, gpointer user_data);
void on_button_valider_emplois101_clicked(GtkWidget *button, gpointer user_data);
void afficher_profile_entraineur(const char *nom, const char *id);
void charger_clients_entraineur(GtkWidget *treeview, const char *entraineur_id);
void charger_emploi_entraineur(GtkWidget *treeview, const char *entraineur_id);
void sauvegarder_progression_clients(GtkWidget *treeview, const char *entraineur_id);
void sauvegarder_emploi_entraineur(GtkWidget *treeview, const char *entraineur_id);



void
on_button51_clicked                    (GtkButton       *button,
                                        gpointer         user_data);
