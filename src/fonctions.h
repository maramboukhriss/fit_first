#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#include <stdio.h>

// ================= STRUCTURE UTILISATEUR =================
typedef struct {
    char id[100];
    char nom[100];
    char prenom[100];
    char email[100];
    char mot_de_passe[100];
    char telephone[100];
    char adresse[100];
    int jour_naissance;
    int mois_naissance;
    int annee_naissance;
    int sexe;           // Radio button: 0=Femme, 1=Homme
    char poids[20];     // Champ poids
} User;
// ================= PROTOTYPES DES FONCTIONS =================

// Fonctions utilisateurs
int ajouter_user(char *filename, User u);
int modifier_user(char *filename, char *id, User nouv);
int supprimer_user(char *filename, char *id);
User chercher_user(char *filename, char *id);
int id_existe_dans_fichier(const char *fichier, const char *id);
// ================= STRUCTURE DEMANDE COACH =================
typedef struct {
    char id_membre[100];  // ID du membre (chaîne de caractères)
    int objectifs[5];      // Check buttons (0 ou 1)
    int disponibilite;     // Radio button: 0=Matin, 1=Midi, 2=Soir
    int jours[5];         // Check buttons (0 ou 1)
} DemandeCoach;

// ================= PROTOTYPES DES FONCTIONS =================


// Fonctions demande coach
void saisir_demande_coach(DemandeCoach *demande);
int enregistrer_demande_coach(DemandeCoach demande);
User chercher_user(char *filename, char *id);
// Fonctions de réservation
DemandeCoach* lire_toutes_demandes(const char *filename, int *count);
int supprimer_reservation_par_ligne(const char *filename, int ligne_a_supprimer);
int texte_vers_disponibilite(const char *texte);
int mettre_a_jour_reservation(DemandeCoach nouvelle_demande, int ligne_index);
DemandeCoach lire_reservation_par_index(const char *filename, int index);
int envoyer_email_inscription(User nouveau_membre);
// Nouvelle structure pour les statistiques
typedef struct {
    int total_membres;
    int hommes;
    int femmes;
    int moins_20_ans;
    int entre_20_30;
    int entre_30_40;
    int entre_40_50;
    int plus_50_ans;
    float age_moyen;
    int age_min;
    int age_max;
} StatistiquesDemographiques;

// Prototypes
StatistiquesDemographiques calculer_statistiques_demographiques();
void generer_rapport_statistiques(const char* filename);

// Structure pour les données de l'histogramme
typedef struct {
    int tranches_age[5];        // [<20, 20-29, 30-39, 40-49, 50+]
    int hommes;
    int femmes;
    int categories_poids[5];    // [<60, 60-69, 70-79, 80-89, 90+]
} HistogrammeData;


// Fonctions histogramme
HistogrammeData calculer_donnees_histogramme();
void dessiner_histogramme(void *cr_ptr, int width, int height, HistogrammeData data);


//centre
typedef struct
{
    int id;
    char nom[50];
    char adresse[100];
    char telephone[20];
    char specialisation[100];
    char capacite[50];
    char horaires_matin[20];
    char horaires_midi[20];
    char horaires_soir[20];
    char type[20];
    char photo_path[100];
} centre;


typedef struct {
    int id_inscription;
    int id_centre;
    int id_coach;
    char date[11]; // format : JJ/MM/AAAA
} inscription;
// Le type de retour int c'est pour indiquer si la tâche a été réalisée avec succès ou non
// pour afficher des messages plus tard exemple erreur d'ouverture de fichier, élément introuvable
int ajouterCentre(char *filename, centre c);
int modifierCentre(char *filename, int id, centre nouv);
int supprimerCentre(char *filename, int id);
centre chercherCentre(char *filename, int id);

int ajouterInscription(char *filename, inscription ins);
int verifierInscriptionExiste(char *filename, int id_centre, int id_coach, char *date);
int genererIdInscription();
void afficherInscriptions(char *filename);
//entraineur
typedef struct {
    char id[20];
    char nom_complet[100];      // CHANGÉ: nom et prénom ensemble
    char specialite[50];
    char email[100];
    char experience[50];
    char reseaux_sociaux[100];
    char statut[20];
    char jours[100];
    char disponibilite[50];
} Entraineur;

Entraineur chercher_entraineur(const char *id);
void ajouter_entraineur(Entraineur e);
void modifier_entraineur(const char *id, Entraineur nouvelles_info);
void supprimer_entraineur(const char *id);
void vider_treeview(GtkWidget *treeview);
void afficher_tous_entraineurs(GtkWidget *treeview);
void configurer_columns_treeview(GtkWidget *treeview);

typedef struct {
    char nom_complet[100];
    char identifiant[20];
    char cours[50];
    char horaire[50];
    char prix[20];
    char salle[50];
} Inscription;

void enregistrer_inscription(Inscription i);

typedef struct {
    int cardio;
    int dance;
    int musculation;
    int boxe;
    int gymnastique;
} Specialites;

// Fonctions pour le profil d'entraîneur
void configurer_columns_profile_client(GtkWidget *treeview);
void configurer_columns_profile_emploi(GtkWidget *treeview);

void charger_clients_entraineur(GtkWidget *treeview, const char *entraineur_id);
void charger_emploi_entraineur(GtkWidget *treeview, const char *entraineur_id);

void sauvegarder_progression_clients(GtkWidget *treeview, const char *entraineur_id);
void sauvegarder_emploi_entraineur(GtkWidget *treeview, const char *entraineur_id);

void afficher_profile_entraineur(const char *nom, const char *id);

// Fonctions utilitaires
void parser_specialites(const char *specialite_str, Specialites *spec);
int correspond_specialites(const char *objectifs_str, const Specialites *spec_entraineur);

#endif
