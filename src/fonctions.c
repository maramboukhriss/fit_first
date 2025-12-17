#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h> // Pour obtenir l'année actuelle
#include <cairo.h>

// Fonction pour calculer l'âge à partir de la date de naissance
int calculer_age(int annee_naissance, int mois_naissance, int jour_naissance) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    int annee_actuelle = local->tm_year + 1900;
    int mois_actuel = local->tm_mon + 1;
    int jour_actuel = local->tm_mday;
    
    int age = annee_actuelle - annee_naissance;
    
    // Ajuster si l'anniversaire n'est pas encore passé cette année
    if (mois_actuel < mois_naissance || 
        (mois_actuel == mois_naissance && jour_actuel < jour_naissance)) {
        age--;
    }
    
    return age;
}

// Fonction principale pour calculer les statistiques
StatistiquesDemographiques calculer_statistiques_demographiques() {
    StatistiquesDemographiques stats = {0};
    FILE *f = fopen("users.txt", "r");
    
    if (f == NULL) {
        printf("Fichier users.txt non trouvé\n");
        return stats;
    }
    
    User user;
    int somme_ages = 0;
    stats.age_min = 100;
    stats.age_max = 0;
    
    while (fscanf(f, "%s %s %s %s %s %s %s %d %d %d %d %s\n",
                 user.id, user.nom, user.prenom, user.email, user.mot_de_passe,
                 user.telephone, user.adresse, &user.jour_naissance, &user.mois_naissance,
                 &user.annee_naissance, &user.sexe, user.poids) != EOF) {
        
        stats.total_membres++;
        
        // Compter hommes/femmes
        if (user.sexe == 0) {
            stats.femmes++;
        } else {
            stats.hommes++;
        }
        
        // Calculer l'âge
        int age = calculer_age(user.annee_naissance, user.mois_naissance, user.jour_naissance);
        somme_ages += age;
        
        // Mettre à jour min/max
        if (age < stats.age_min) stats.age_min = age;
        if (age > stats.age_max) stats.age_max = age;
        
        // Répartition par tranche d'âge
        if (age < 20) stats.moins_20_ans++;
        else if (age >= 20 && age < 30) stats.entre_20_30++;
        else if (age >= 30 && age < 40) stats.entre_30_40++;
        else if (age >= 40 && age < 50) stats.entre_40_50++;
        else stats.plus_50_ans++;
    }
    
    fclose(f);
    
    // Calculer l'âge moyen
    if (stats.total_membres > 0) {
        stats.age_moyen = (float)somme_ages / stats.total_membres;
    }
    
    return stats;
}

// Fonction pour générer un rapport texte
void generer_rapport_statistiques(const char* filename) {
    StatistiquesDemographiques stats = calculer_statistiques_demographiques();
    
    FILE *rapport = fopen(filename, "w");
    if (rapport == NULL) {
        printf("Erreur création du rapport\n");
        return;
    }
    
    fprintf(rapport, "=== RAPPORT STATISTIQUES - FITNESS CLUB ===\n\n");
    fprintf(rapport, "Date de génération : %s\n\n", __DATE__);
    
    fprintf(rapport, "📊 STATISTIQUES DÉMOGRAPHIQUES\n");
    fprintf(rapport, "--------------------------------\n");
    fprintf(rapport, "Total membres : %d\n", stats.total_membres);
    fprintf(rapport, "Hommes : %d (%.1f%%)\n", stats.hommes, 
            stats.total_membres > 0 ? (float)stats.hommes/stats.total_membres*100 : 0);
    fprintf(rapport, "Femmes : %d (%.1f%%)\n", stats.femmes,
            stats.total_membres > 0 ? (float)stats.femmes/stats.total_membres*100 : 0);
    fprintf(rapport, "Âge moyen : %.1f ans\n", stats.age_moyen);
    fprintf(rapport, "Âge minimum : %d ans\n", stats.age_min);
    fprintf(rapport, "Âge maximum : %d ans\n\n", stats.age_max);
    
    fprintf(rapport, "📈 RÉPARTITION PAR ÂGE\n");
    fprintf(rapport, "----------------------\n");
    fprintf(rapport, "Moins de 20 ans : %d (%.1f%%)\n", stats.moins_20_ans,
            stats.total_membres > 0 ? (float)stats.moins_20_ans/stats.total_membres*100 : 0);
    fprintf(rapport, "20-29 ans : %d (%.1f%%)\n", stats.entre_20_30,
            stats.total_membres > 0 ? (float)stats.entre_20_30/stats.total_membres*100 : 0);
    fprintf(rapport, "30-39 ans : %d (%.1f%%)\n", stats.entre_30_40,
            stats.total_membres > 0 ? (float)stats.entre_30_40/stats.total_membres*100 : 0);
    fprintf(rapport, "40-49 ans : %d (%.1f%%)\n", stats.entre_40_50,
            stats.total_membres > 0 ? (float)stats.entre_40_50/stats.total_membres*100 : 0);
    fprintf(rapport, "50 ans et plus : %d (%.1f%%)\n", stats.plus_50_ans,
            stats.total_membres > 0 ? (float)stats.plus_50_ans/stats.total_membres*100 : 0);
    
    fclose(rapport);
    printf("Rapport généré : %s\n", filename);
}

// ================= FONCTIONS UTILISATEURS =================
int id_existe_dans_fichier(const char *fichier, const char *id) {
    FILE *f = fopen(fichier, "r");
    if (f == NULL) {
        return 0; // Fichier n'existe pas encore
    }
    
    User u;
    int id_existe = 0;
    
    while (fscanf(f, "%s %s %s %s %s %s %s %d %d %d %d %s\n",
                 u.id, u.nom, u.prenom, u.email, u.mot_de_passe,
                 u.telephone, u.adresse, &u.jour_naissance, &u.mois_naissance,
                 &u.annee_naissance, &u.sexe, u.poids) != EOF) {
        
        if (strcmp(u.id, id) == 0) {
            id_existe = 1;
            break;
        }
    }
    
    fclose(f);
    return id_existe;
}
int ajouter_user(char *filename, User u) {
    FILE *f = fopen(filename, "a");
    if (f != NULL) {
        fprintf(f, "%s %s %s %s %s %s %s %d %d %d %d %s\n",
                u.id, u.nom, u.prenom, u.email, u.mot_de_passe,
                u.telephone, u.adresse, u.jour_naissance, u.mois_naissance,
                u.annee_naissance, u.sexe, u.poids);
        fclose(f);
        return 1;
    }
    return 0;
}

int modifier_user(char *filename, char *id, User nouv) {
    int tr = 0;
    User u;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp_user.txt", "w");
    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%s %s %s %s %s %s %s %d %d %d %d %s\n",
                     u.id, u.nom, u.prenom, u.email, u.mot_de_passe,
                     u.telephone, u.adresse, &u.jour_naissance, &u.mois_naissance,
                     &u.annee_naissance, &u.sexe, u.poids) != EOF) {
            if (strcmp(u.id, id) == 0) {
                fprintf(f2, "%s %s %s %s %s %s %s %d %d %d %d %s\n",
                        nouv.id, nouv.nom, nouv.prenom, nouv.email, nouv.mot_de_passe,
                        nouv.telephone, nouv.adresse, nouv.jour_naissance, nouv.mois_naissance,
                        nouv.annee_naissance, nouv.sexe, nouv.poids);
                tr = 1;
            } else {
                fprintf(f2, "%s %s %s %s %s %s %s %d %d %d %d %s\n",
                        u.id, u.nom, u.prenom, u.email, u.mot_de_passe,
                        u.telephone, u.adresse, u.jour_naissance, u.mois_naissance,
                        u.annee_naissance, u.sexe, u.poids);
            }
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("temp_user.txt", filename);
    }
    return tr;
}

int supprimer_user(char *filename, char *id) {
    int tr = 0;
    User u;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp_user.txt", "w");
    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%s %s %s %s %s %s %s %d %d %d %d %s\n",
                     u.id, u.nom, u.prenom, u.email, u.mot_de_passe,
                     u.telephone, u.adresse, &u.jour_naissance, &u.mois_naissance,
                     &u.annee_naissance, &u.sexe, u.poids) != EOF) {
            if (strcmp(u.id, id) == 0) {
                tr = 1;
            } else {
                fprintf(f2, "%s %s %s %s %s %s %s %d %d %d %d %s\n",
                        u.id, u.nom, u.prenom, u.email, u.mot_de_passe,
                        u.telephone, u.adresse, u.jour_naissance, u.mois_naissance,
                        u.annee_naissance, u.sexe, u.poids);
            }
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("temp_user.txt", filename);
    }
    return tr;
}


// Fonction pour nettoyer une chaîne (supprimer espaces et retours à la ligne)
static void clean_string(char *str) {
    if (str == NULL) return;
    
    // Supprimer les espaces/retours en début
    char *start = str;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')) {
        start++;
    }
    
    // Si la chaîne commence par des espaces, décaler
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    
    // Supprimer les espaces/retours en fin
    char *end = str + strlen(str) - 1;
    while (end >= str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }
}

User chercher_user(char *filename, char *id) {
    User u;
    int tr = 0;
    
    // Nettoyer l'ID recherché
    char id_clean[100];
    strncpy(id_clean, id, sizeof(id_clean) - 1);
    id_clean[sizeof(id_clean) - 1] = '\0';
    clean_string(id_clean);
    
    printf("DEBUG chercher_user: Recherche de l'ID '%s' dans %s\n", id_clean, filename);
    
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("ERREUR: Impossible d'ouvrir le fichier %s\n", filename);
        strcpy(u.id, "-1");
        return u;
    }
    
    char line[1024];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), f)) {
        line_num++;
        
        // Nettoyer la ligne
        clean_string(line);
        
        // Si la ligne est vide, passer à la suivante
        if (strlen(line) == 0) continue;
        
        printf("DEBUG chercher_user: Ligne %d: '%s'\n", line_num, line);
        
        // Essayer de parser la ligne avec sscanf
        // Note: nous utilisons %99s pour éviter le débordement
        int result = sscanf(line, "%99s %99s %99s %99s %99s %99s %99[^\n] %d %d %d %d %99s",
                          u.id, u.nom, u.prenom, u.email, u.mot_de_passe,
                          u.telephone, u.adresse, &u.jour_naissance, &u.mois_naissance,
                          &u.annee_naissance, &u.sexe, u.poids);
        
        printf("DEBUG chercher_user: Résultat sscanf: %d champs\n", result);
        
        // Si nous avons lu 12 champs, comparer les IDs
        if (result == 12) {
            // Nettoyer l'ID lu du fichier
            clean_string(u.id);
            
            printf("DEBUG chercher_user: ID lu: '%s', ID recherché: '%s'\n", u.id, id_clean);
            
            if (strcmp(u.id, id_clean) == 0) {
                tr = 1;
                printf("DEBUG chercher_user: ID TROUVÉ à la ligne %d\n", line_num);
                break;
            }
        } else if (result >= 7) {
            // Tentative alternative: peut-être que l'adresse contient des espaces
            // Dans ce cas, nous réessayons avec une adresse simple (un seul mot)
            result = sscanf(line, "%99s %99s %99s %99s %99s %99s %99s %d %d %d %d %99s",
                          u.id, u.nom, u.prenom, u.email, u.mot_de_passe,
                          u.telephone, u.adresse, &u.jour_naissance, &u.mois_naissance,
                          &u.annee_naissance, &u.sexe, u.poids);
            
            if (result == 12) {
                clean_string(u.id);
                printf("DEBUG chercher_user (2e essai): ID lu: '%s', ID recherché: '%s'\n", u.id, id_clean);
                
                if (strcmp(u.id, id_clean) == 0) {
                    tr = 1;
                    printf("DEBUG chercher_user: ID TROUVÉ (2e essai) à la ligne %d\n", line_num);
                    break;
                }
            }
        }
    }
    
    fclose(f);
    
    if (!tr) {
        printf("DEBUG chercher_user: ID '%s' NON TROUVÉ\n", id_clean);
        strcpy(u.id, "-1");
    } else {
        printf("DEBUG chercher_user: Succès - Membre trouvé: %s %s\n", u.prenom, u.nom);
    }
    
    return u;
}

// Fonction utilitaire pour vider le buffer d'entrée
static void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void saisir_demande_coach(DemandeCoach *demande) {
    int i;
    int result;
    
    printf("\n=== DEMANDE DE COACH PRIVE ===\n");
    
    // Initialiser toutes les cases à 0
    for(i = 0; i < 5; i++) demande->objectifs[i] = 0;
    for(i = 0; i < 5; i++) demande->jours[i] = 0;
    
    // Saisie de l'ID du membre (vérification d'abord)
    printf("ID du membre: ");
    result = scanf("%s", demande->id_membre);
    if (result != 1) {
        printf("❌ ERREUR: Saisie de l'ID invalide!\n");
        vider_buffer();
        strcpy(demande->id_membre, "-1");
        return;
    }
    
    // Vérifier si le membre existe
    User membre = chercher_user("users.txt", demande->id_membre);
    if (strcmp(membre.id, "-1") == 0) {
        printf("❌ ERREUR: Aucun membre trouvé avec cet ID!\n");
        strcpy(demande->id_membre, "-1");
        return;
    }
    printf("✅ Membre trouvé: %s %s\n", membre.prenom, membre.nom);
    
    // Saisie des objectifs (check buttons)
    printf("\n=== OBJECTIFS DE COACHING (cocher 1 pour OUI, 0 pour NON) ===\n");
    
    printf("[0] Perte de poids: ");
    result = scanf("%d", &demande->objectifs[0]);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour l'objectif 0!\n");
        vider_buffer();
        return;
    }
    
    printf("[1] Prise de muscle: ");
    result = scanf("%d", &demande->objectifs[1]);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour l'objectif 1!\n");
        vider_buffer();
        return;
    }
    
    printf("[2] Cardio: ");
    result = scanf("%d", &demande->objectifs[2]);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour l'objectif 2!\n");
        vider_buffer();
        return;
    }
    
    printf("[3] Amélioration des performances: ");
    result = scanf("%d", &demande->objectifs[3]);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour l'objectif 3!\n");
        vider_buffer();
        return;
    }
    
    printf("[4] Gymnastique: ");
    result = scanf("%d", &demande->objectifs[4]);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour l'objectif 4!\n");
        vider_buffer();
        return;
    }
    
    // Saisie de la disponibilité (radio button)
    printf("\n=== DISPONIBILITE (choisir 0, 1 ou 2) ===\n");
    printf("[0] Matin\n");
    printf("[1] Midi\n");
    printf("[2] Soir\n");
    printf("Votre choix: ");
    result = scanf("%d", &demande->disponibilite);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour la disponibilité!\n");
        vider_buffer();
        return;
    }
    
    // Validation de la disponibilité
    if (demande->disponibilite < 0 || demande->disponibilite > 2) {
        printf("❌ ERREUR: Disponibilité doit être 0, 1 ou 2!\n");
        return;
    }
    
    // Saisie des jours (check buttons)
    printf("\n=== JOURS D'ENTRAINEMENT (cocher 1 pour OUI, 0 pour NON) ===\n");
    
    printf("[0] LUNDI: ");
    result = scanf("%d", &demande->jours[0]);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour le jour 0!\n");
        vider_buffer();
        return;
    }
    
    printf("[1] MARDI: ");
    result = scanf("%d", &demande->jours[1]);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour le jour 1!\n");
        vider_buffer();
        return;
    }
    
    printf("[2] MERCREDI: ");
    result = scanf("%d", &demande->jours[2]);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour le jour 2!\n");
        vider_buffer();
        return;
    }
    
    printf("[3] JEUDI: ");
    result = scanf("%d", &demande->jours[3]);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour le jour 3!\n");
        vider_buffer();
        return;
    }
    
    printf("[4] VENDREDI: ");
    result = scanf("%d", &demande->jours[4]);
    if (result != 1) {
        printf("❌ ERREUR: Saisie invalide pour le jour 4!\n");
        vider_buffer();
        return;
    }
    
    printf("✅ Saisie terminée avec succès!\n");
}

int enregistrer_demande_coach(DemandeCoach demande) {
    FILE *f = fopen("reservation.txt", "a");
    if (f == NULL) {
        printf("❌ Erreur: Impossible de créer/ouvrir le fichier reservation.txt\n");
        return 0;
    }
    
    // Convertir les tableaux en chaînes pour l'enregistrement
    char objectifs_str[100] = "";
    char jours_str[100] = "";
    
    int i;
    for(i = 0; i < 5; i++) {
        char temp[10];
        snprintf(temp, sizeof(temp), "%d", demande.objectifs[i]);
        strncat(objectifs_str, temp, sizeof(objectifs_str) - strlen(objectifs_str) - 1);
        if(i < 4) strncat(objectifs_str, ",", sizeof(objectifs_str) - strlen(objectifs_str) - 1);
        
        snprintf(temp, sizeof(temp), "%d", demande.jours[i]);
        strncat(jours_str, temp, sizeof(jours_str) - strlen(jours_str) - 1);
        if(i < 4) strncat(jours_str, ",", sizeof(jours_str) - strlen(jours_str) - 1);
    }
    
    // Convertir la disponibilité en texte
    char *dispo_str;
    switch(demande.disponibilite) {
        case 0: dispo_str = "Matin"; break;
        case 1: dispo_str = "Midi"; break;
        case 2: dispo_str = "Soir"; break;
        default: dispo_str = "Inconnu";
    }
    
    // Enregistrer dans le fichier
    fprintf(f, "Membre:%s | Objectifs:%s | Disponibilite:%s | Jours:%s\n",
            demande.id_membre, objectifs_str, dispo_str, jours_str);
    fclose(f);
    
    printf("\n✅ Demande de coach enregistrée avec succès!\n");
    printf("📁 Fichier: reservation.txt\n");
    
    return 1; // Succès
}

// Fonction pour supprimer une réservation par index de ligne
int supprimer_reservation_par_ligne(const char *filename, int ligne_a_supprimer) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return 0;
    }
    
    FILE *temp = fopen("temp_reservation.txt", "w");
    if (temp == NULL) {
        fclose(f);
        return 0;
    }
    
    char line[1024];
    int ligne_courante = 0;
    int supprime = 0;
    
    while (fgets(line, sizeof(line), f)) {
        if (ligne_courante != ligne_a_supprimer) {
            fputs(line, temp);
        } else {
            supprime = 1;
        }
        ligne_courante++;
    }
    
    fclose(f);
    fclose(temp);
    
    if (supprime) {
        remove(filename);
        rename("temp_reservation.txt", filename);
        return 1;
    } else {
        remove("temp_reservation.txt");
        return 0;
    }
}
DemandeCoach* lire_toutes_demandes(const char *filename, int *count) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        *count = 0;
        return NULL;
    }
    
    // Première passe pour compter le nombre de lignes
    char line[1024];
    int line_count = 0;
    while (fgets(line, sizeof(line), f)) {
        line_count++;
    }
    rewind(f);
    
    // Allouer mémoire
    DemandeCoach *demandes = malloc(line_count * sizeof(DemandeCoach));
    if (demandes == NULL) {
        fclose(f);
        *count = 0;
        return NULL;
    }
    
    int index = 0;
    while (fgets(line, sizeof(line), f)) {
        // Nettoyer la ligne
        line[strcspn(line, "\n")] = 0;
        line[strcspn(line, "\r")] = 0;
        
        // DEBUG: Afficher la ligne lue
        printf("DEBUG: Parsing line: %s\n", line);
        
        // Extraire les parties avec sscanf
        char id[100] = "";
        char objectifs_str[100] = "";
        char disponibilite_str[50] = "";
        char jours_str[100] = "";
        
        // Utiliser un format qui correspond exactement à votre fichier
        // Le format: Membre:ID | Objectifs:0,1,0,0,0 | Disponibilite:Matin | Jours:0,0,0,1,0
        if (sscanf(line, "Membre:%99[^|] | Objectifs:%99[^|] | Disponibilite:%99[^|] | Jours:%99[^\n]", 
                   id, objectifs_str, disponibilite_str, jours_str) == 4) {
            
            // Nettoyer les espaces autour des chaînes
            char *clean_id = id;
            while (*clean_id == ' ') clean_id++;
            char *end_id = clean_id + strlen(clean_id) - 1;
            while (end_id > clean_id && *end_id == ' ') end_id--;
            *(end_id + 1) = '\0';
            
            char *clean_disp = disponibilite_str;
            while (*clean_disp == ' ') clean_disp++;
            char *end_disp = clean_disp + strlen(clean_disp) - 1;
            while (end_disp > clean_disp && *end_disp == ' ') end_disp--;
            *(end_disp + 1) = '\0';
            
            // DEBUG
            printf("DEBUG: Extracted - ID:'%s', Disp:'%s'\n", clean_id, clean_disp);
            
            // Copier l'ID
            strcpy(demandes[index].id_membre, clean_id);
            
            // Convertir disponibilité
            if (strcmp(clean_disp, "Matin") == 0) {
                demandes[index].disponibilite = 0;
            } else if (strcmp(clean_disp, "Midi") == 0) {
                demandes[index].disponibilite = 1;
            } else if (strcmp(clean_disp, "Soir") == 0) {
                demandes[index].disponibilite = 2;
            } else {
                printf("WARNING: Disponibilite inconnue: '%s'\n", clean_disp);
                demandes[index].disponibilite = -1;
            }
            
            // Convertir objectifs
            int obj_values[5] = {0};
            int obj_count = 0;
            char *token = strtok(objectifs_str, ",");
            while (token != NULL && obj_count < 5) {
                // Nettoyer le token
                char *clean_token = token;
                while (*clean_token == ' ') clean_token++;
                char *end_token = clean_token + strlen(clean_token) - 1;
                while (end_token > clean_token && *end_token == ' ') end_token--;
                *(end_token + 1) = '\0';
                
                obj_values[obj_count] = atoi(clean_token);
                obj_count++;
                token = strtok(NULL, ",");
            }
            for (int i = 0; i < 5; i++) {
                demandes[index].objectifs[i] = obj_values[i];
            }
            
            // Convertir jours
            int jours_values[5] = {0};
            int jours_count = 0;
            token = strtok(jours_str, ",");
            while (token != NULL && jours_count < 5) {
                char *clean_token = token;
                while (*clean_token == ' ') clean_token++;
                char *end_token = clean_token + strlen(clean_token) - 1;
                while (end_token > clean_token && *end_token == ' ') end_token--;
                *(end_token + 1) = '\0';
                
                jours_values[jours_count] = atoi(clean_token);
                jours_count++;
                token = strtok(NULL, ",");
            }
            for (int i = 0; i < 5; i++) {
                demandes[index].jours[i] = jours_values[i];
            }
            
            index++;
        } else {
            printf("WARNING: Failed to parse line: %s\n", line);
            printf("DEBUG: sscanf returned %d items\n", 
                   sscanf(line, "Membre:%99[^|] | Objectifs:%99[^|] | Disponibilite:%99[^|] | Jours:%99[^\n]", 
                          id, objectifs_str, disponibilite_str, jours_str));
        }
    }
    
    fclose(f);
    *count = index;
    
    printf("DEBUG: Total reservations read: %d\n", index);
    return demandes;
}
// Fonction pour mettre à jour une réservation par index de ligne
int mettre_a_jour_reservation(DemandeCoach nouvelle_demande, int ligne_index) {
    FILE *f = fopen("reservation.txt", "r");
    if (f == NULL) {
        printf("ERREUR: Impossible d'ouvrir reservation.txt pour lecture\n");
        return 0;
    }
    
    FILE *temp = fopen("temp_reservation.txt", "w");
    if (temp == NULL) {
        fclose(f);
        printf("ERREUR: Impossible de créer fichier temporaire\n");
        return 0;
    }
    
    char line[1024];
    int ligne_courante = 0;
    int modifie = 0;
    
    while (fgets(line, sizeof(line), f)) {
        // Supprimer le saut de ligne
        line[strcspn(line, "\n")] = 0;
        line[strcspn(line, "\r")] = 0;
        
        if (ligne_courante == ligne_index) {
            // Écrire la nouvelle réservation
            // Convertir les tableaux en chaînes
            char objectifs_str[100] = "";
            char jours_str[100] = "";
            
            for (int i = 0; i < 5; i++) {
                char temp_str[10];
                snprintf(temp_str, sizeof(temp_str), "%d", nouvelle_demande.objectifs[i]);
                strncat(objectifs_str, temp_str, sizeof(objectifs_str) - strlen(objectifs_str) - 1);
                if (i < 4) strncat(objectifs_str, ",", sizeof(objectifs_str) - strlen(objectifs_str) - 1);
                
                snprintf(temp_str, sizeof(temp_str), "%d", nouvelle_demande.jours[i]);
                strncat(jours_str, temp_str, sizeof(jours_str) - strlen(jours_str) - 1);
                if (i < 4) strncat(jours_str, ",", sizeof(jours_str) - strlen(jours_str) - 1);
            }
            
            // Convertir la disponibilité en texte
            const char *dispo_str;
            switch(nouvelle_demande.disponibilite) {
                case 0: dispo_str = "Matin"; break;
                case 1: dispo_str = "Midi"; break;
                case 2: dispo_str = "Soir"; break;
                default: dispo_str = "Inconnu";
            }
            
            fprintf(temp, "Membre:%s | Objectifs:%s | Disponibilite:%s | Jours:%s\n",
                    nouvelle_demande.id_membre, objectifs_str, dispo_str, jours_str);
            modifie = 1;
        } else {
            // Recopier la ligne telle quelle
            fprintf(temp, "%s\n", line);
        }
        
        ligne_courante++;
    }
    
    fclose(f);
    fclose(temp);
    
    if (modifie) {
        // Remplacer l'ancien fichier par le nouveau
        remove("reservation.txt");
        rename("temp_reservation.txt", "reservation.txt");
        return 1;
    } else {
        remove("temp_reservation.txt");
        return 0;
    }
}

// Fonction pour convertir le texte en disponibilité
int texte_vers_disponibilite(const char *texte) {
    if (strcmp(texte, "Matin") == 0) return 0;
    if (strcmp(texte, "Midi") == 0) return 1;
    if (strcmp(texte, "Soir") == 0) return 2;
    return -1;
}

// Fonction pour lire une réservation spécifique par index
DemandeCoach lire_reservation_par_index(const char *filename, int index) {
    DemandeCoach demande;
    int count = 0;
    DemandeCoach *demandes = lire_toutes_demandes(filename, &count);
    
    if (demandes == NULL || index < 0 || index >= count) {
        strcpy(demande.id_membre, "-1");
        return demande;
    }
    
    demande = demandes[index];
    free(demandes);
    return demande;
}
// Fonction pour envoyer un email d'inscription
int envoyer_email_inscription(User nouveau_membre) {
    // Créer un dossier pour les confirmations
    mkdir("confirmations", 0755);
    
    // Nom du fichier
    char filename[256];
    snprintf(filename, sizeof(filename), 
             "confirmations/%s_inscription.txt", 
             nouveau_membre.id);
    
    // Ouvrir le fichier
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Erreur: impossible de créer le fichier de confirmation\n");
        return 0;
    }
    
    // Écrire la confirmation
    fprintf(f, "=== CONFIRMATION D'INSCRIPTION ===\n\n");
    fprintf(f, "Date: %s\n", __DATE__);
    fprintf(f, "Heure: %s\n", __TIME__);
    fprintf(f, "\n");
    fprintf(f, "DESTINATAIRE:\n");
    fprintf(f, "  %s %s\n", nouveau_membre.prenom, nouveau_membre.nom);
    fprintf(f, "  Email: %s\n", nouveau_membre.email);
    fprintf(f, "  Téléphone: %s\n", nouveau_membre.telephone);
    fprintf(f, "\n");
    fprintf(f, "DÉTAILS DE L'INSCRIPTION:\n");
    fprintf(f, "  ID Membre: %s\n", nouveau_membre.id);
    fprintf(f, "  Statut: CONFIRMÉ\n");
    fprintf(f, "\n");
    fprintf(f, "En production, cet email serait envoyé à l'adresse ci-dessus.\n");
    
    fclose(f);
    
    // Afficher confirmation dans le terminal
    printf("\n");
    printf("=========================================\n");
    printf("✅ INSCRIPTION CONFIRMÉE\n");
    printf("=========================================\n");
    printf("Membre: %s %s\n", nouveau_membre.prenom, nouveau_membre.nom);
    printf("ID: %s\n", nouveau_membre.id);
    printf("Email: %s\n", nouveau_membre.email);
    printf("Fichier de confirmation: %s\n", filename);
    printf("=========================================\n\n");
    
    return 1;
}
// Fonction pour calculer les données de l'histogramme
HistogrammeData calculer_donnees_histogramme() {
    HistogrammeData data = {0};
    
    FILE *f = fopen("users.txt", "r");
    if (f == NULL) {
        printf("Fichier users.txt non trouvé\n");
        return data;
    }
    
    User user;
    while (fscanf(f, "%s %s %s %s %s %s %s %d %d %d %d %s\n",
                 user.id, user.nom, user.prenom, user.email, user.mot_de_passe,
                 user.telephone, user.adresse, &user.jour_naissance, &user.mois_naissance,
                 &user.annee_naissance, &user.sexe, user.poids) != EOF) {
        
        // Calculer l'âge
        int age = calculer_age(user.annee_naissance, user.mois_naissance, user.jour_naissance);
        
        // Classer par tranche d'âge
        if (age < 20) data.tranches_age[0]++;
        else if (age >= 20 && age < 30) data.tranches_age[1]++;
        else if (age >= 30 && age < 40) data.tranches_age[2]++;
        else if (age >= 40 && age < 50) data.tranches_age[3]++;
        else data.tranches_age[4]++;
        
        // Par sexe
        if (user.sexe == 0) data.femmes++;
        else data.hommes++;
        
        // Par poids
        float poids = atof(user.poids);
        if (poids > 0) {
            if (poids < 60) data.categories_poids[0]++;
            else if (poids < 70) data.categories_poids[1]++;
            else if (poids < 80) data.categories_poids[2]++;
            else if (poids < 90) data.categories_poids[3]++;
            else data.categories_poids[4]++;
        }
    }
    
    fclose(f);
    return data;
}

// Fonction pour dessiner un histogramme avec Cairo (GTK2 compatible)
// Fonction pour dessiner un histogramme avec Cairo (GTK2 compatible)
void dessiner_histogramme(void *cr_ptr, int width, int height, HistogrammeData data) {
    // Convertir le pointeur void en cairo_t
    cairo_t *cr = (cairo_t *)cr_ptr;
    
    // Fond blanc
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    
    // Titre
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 18);
    cairo_move_to(cr, width/2 - 100, 30);
    cairo_show_text(cr, "Histogramme des Membres");
    
    // Sous-titre
    cairo_set_font_size(cr, 12);
    cairo_move_to(cr, width/2 - 80, 50);
    cairo_show_text(cr, "Fitness First Club");
    
    // ===== HISTOGRAMME DES ÂGES =====
    cairo_set_font_size(cr, 14);
    cairo_move_to(cr, 50, 80);
    cairo_show_text(cr, "Repartition par age");
    
    // Coordonnées de l'histogramme
    int start_x = 50;
    int start_y = 120;
    int graph_width = width - 100;
    int graph_height = 200;
    int bar_width = (graph_width - 60) / 5;
    
    // Trouver la valeur maximale pour l'échelle
    int max_value = 0;
    for (int i = 0; i < 5; i++) {
        if (data.tranches_age[i] > max_value) {
            max_value = data.tranches_age[i];
        }
    }
    if (max_value == 0) max_value = 1; // Éviter la division par zéro
    
    // Dessiner les axes
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1);
    
    // Axe vertical (Y)
    cairo_move_to(cr, start_x, start_y);
    cairo_line_to(cr, start_x, start_y + graph_height);
    
    // Axe horizontal (X)
    cairo_move_to(cr, start_x, start_y + graph_height);
    cairo_line_to(cr, start_x + graph_width, start_y + graph_height);
    
    cairo_stroke(cr);
    
    // Échelle sur l'axe Y
    cairo_set_font_size(cr, 10);
    for (int i = 0; i <= 10; i++) {
        int y = start_y + graph_height - (i * graph_height / 10);
        cairo_move_to(cr, start_x - 30, y);
        
        // CORRECTION ICI : Utiliser snprintf au lieu de sprintf
        char label[12]; // Augmenter la taille pour éviter le débordement
        int value = (max_value * i) / 10;
        snprintf(label, sizeof(label), "%d", value);
        cairo_show_text(cr, label);
    }
    
    // Labels des catégories d'âge
    const char *labels_age[] = {"<20", "20-29", "30-39", "40-49", "50+"};
    
    // Dessiner les barres
    for (int i = 0; i < 5; i++) {
        int bar_height = (int)((float)data.tranches_age[i] / max_value * graph_height);
        int x = start_x + 30 + i * (bar_width + 10);
        int y = start_y + graph_height - bar_height;
        
        // Couleur de la barre (dégradé de bleu)
        cairo_pattern_t *pat = cairo_pattern_create_linear(x, y, x, y + bar_height);
        cairo_pattern_add_color_stop_rgba(pat, 0, 0.2, 0.4, 0.8, 0.8);
        cairo_pattern_add_color_stop_rgba(pat, 1, 0.1, 0.2, 0.6, 0.8);
        cairo_set_source(cr, pat);
        
        // Dessiner la barre
        cairo_rectangle(cr, x, y, bar_width, bar_height);
        cairo_fill(cr);
        cairo_pattern_destroy(pat);
        
        // Contour de la barre
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width(cr, 0.5);
        cairo_rectangle(cr, x, y, bar_width, bar_height);
        cairo_stroke(cr);
        
        // Label sous la barre
        cairo_move_to(cr, x + bar_width/2 - 10, start_y + graph_height + 15);
        cairo_show_text(cr, labels_age[i]);
        
        // Valeur au-dessus de la barre
        if (data.tranches_age[i] > 0) {
            // CORRECTION ICI : Utiliser snprintf au lieu de sprintf
            char valeur[12]; // Augmenter la taille
            snprintf(valeur, sizeof(valeur), "%d", data.tranches_age[i]);
            cairo_move_to(cr, x + bar_width/2 - 5, y - 5);
            cairo_show_text(cr, valeur);
        }
    }
    
    // ===== HISTOGRAMME DES SEXES =====
    int start_y2 = start_y + graph_height + 50;
    cairo_set_font_size(cr, 14);
    cairo_move_to(cr, 50, start_y2);
    cairo_show_text(cr, "Repartition par sexe");
    
    // Positions pour les barres hommes/femmes
    int bar_width2 = 50;
    int x_hommes = 100;
    int x_femmes = 200;
    int total_sexes = data.hommes + data.femmes;
    
    if (total_sexes > 0) {
        // Barre hommes (bleu)
        int bar_height_hommes = (int)((float)data.hommes / total_sexes * 100);
        cairo_set_source_rgb(cr, 0.1, 0.3, 0.8);
        cairo_rectangle(cr, x_hommes, start_y2 + 30, bar_width2, bar_height_hommes);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, x_hommes, start_y2 + 30, bar_width2, bar_height_hommes);
        cairo_stroke(cr);
        cairo_move_to(cr, x_hommes + 10, start_y2 + bar_height_hommes + 45);
        cairo_show_text(cr, "Hommes");
        
        // Barre femmes (rose)
        int bar_height_femmes = (int)((float)data.femmes / total_sexes * 100);
        cairo_set_source_rgb(cr, 0.9, 0.3, 0.6);
        cairo_rectangle(cr, x_femmes, start_y2 + 30, bar_width2, bar_height_femmes);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, x_femmes, start_y2 + 30, bar_width2, bar_height_femmes);
        cairo_stroke(cr);
        cairo_move_to(cr, x_femmes + 10, start_y2 + bar_height_femmes + 45);
        cairo_show_text(cr, "Femmes");
    }
    
    // Légende
    cairo_set_font_size(cr, 10);
    cairo_move_to(cr, width - 200, start_y2 + 60);
    
    // CORRECTION ICI : Utiliser snprintf au lieu de sprintf
    char legende[100];
    snprintf(legende, sizeof(legende), "Total: %d membres", data.hommes + data.femmes);
    cairo_show_text(cr, legende);
    
    // Date en bas
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_set_font_size(cr, 10);
    
    // CORRECTION ICI : Utiliser snprintf au lieu de sprintf
    char date[50];
    snprintf(date, sizeof(date), "Genere le %s", __DATE__);
    cairo_move_to(cr, width - 150, height - 20);
    cairo_show_text(cr, date);
}

//centre

int ajouterCentre(char *filename, centre c) {
    FILE *f = fopen(filename, "a");  // Mode "a" pour append (ajouter)
    if (f == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }
    
    // Écrire toutes les données séparées par des points-virgules
    fprintf(f, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
            c.id, c.nom, c.adresse, c.telephone, c.specialisation,
            c.capacite, c.horaires_matin, c.horaires_midi, 
            c.horaires_soir, c.type);
    
    fclose(f);
    return 1;
}

int modifierCentre(char *filename, int id, centre nouv)
{
    int tr = 0;
    centre c;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");
    
    if(f != NULL && f2 != NULL)
    {
        while(fscanf(f, "%d;%49[^;];%99[^;];%19[^;];%99[^;];%49[^;];%19[^;];%19[^;];%19[^;];%19[^;];%99[^\n]\n",
                     &c.id, c.nom, c.adresse, c.telephone,
                     c.specialisation, c.capacite,
                     c.horaires_matin, c.horaires_midi, c.horaires_soir,
                     c.type, c.photo_path) != EOF)
        {
            if(c.id == id)
            {
                fprintf(f2, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
                        nouv.id, nouv.nom, nouv.adresse, nouv.telephone,
                        nouv.specialisation, nouv.capacite,
                        nouv.horaires_matin, nouv.horaires_midi, nouv.horaires_soir,
                        nouv.type, nouv.photo_path);
                tr = 1;
            }
            else
            {
                fprintf(f2, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
                        c.id, c.nom, c.adresse, c.telephone,
                        c.specialisation, c.capacite,
                        c.horaires_matin, c.horaires_midi, c.horaires_soir,
                        c.type, c.photo_path);
            }
        }
    }
    
    if(f != NULL) fclose(f);
    if(f2 != NULL) fclose(f2);
    
    if(tr == 1)
    {
        remove(filename);
        rename("temp.txt", filename);
    }
    else
    {
        remove("temp.txt");
    }
    
    return tr;
}

int supprimerCentre(char *filename, int id)
{
    int tr = 0;
    centre c;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");
    
    if(f != NULL && f2 != NULL)
    {
        while(fscanf(f, "%d;%49[^;];%99[^;];%19[^;];%99[^;];%49[^;];%19[^;];%19[^;];%19[^;];%19[^;];%99[^\n]\n",
                     &c.id, c.nom, c.adresse, c.telephone,
                     c.specialisation, c.capacite,
                     c.horaires_matin, c.horaires_midi, c.horaires_soir,
                     c.type, c.photo_path) != EOF)
        {
            if(c.id == id)
            {
                tr = 1;
            }
            else
            {
                fprintf(f2, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
                        c.id, c.nom, c.adresse, c.telephone,
                        c.specialisation, c.capacite,
                        c.horaires_matin, c.horaires_midi, c.horaires_soir,
                        c.type, c.photo_path);
            }
        }
    }
    
    if(f != NULL) fclose(f);
    if(f2 != NULL) fclose(f2);
    
    if(tr == 1)
    {
        remove(filename);
        rename("temp.txt", filename);
    }
    else
    {
        remove("temp.txt");
    }
    
    return tr;
}

centre chercherCentre(char *filename, int id)
{
    centre c;
    int tr = 0;
    FILE *f = fopen(filename, "r");
    
    if(f != NULL)
    {
        while(tr == 0 && fscanf(f, "%d;%49[^;];%99[^;];%19[^;];%99[^;];%49[^;];%19[^;];%19[^;];%19[^;];%19[^;];%99[^\n]\n",
                                &c.id, c.nom, c.adresse, c.telephone,
                                c.specialisation, c.capacite,
                                c.horaires_matin, c.horaires_midi, c.horaires_soir,
                                c.type, c.photo_path) != EOF)
        {
            if(c.id == id)
            {
                tr = 1;
            }
        }
        fclose(f);
    }
    
    if(tr == 0)
        c.id = -1;
    
    return c;
}


int ajouterInscription(char *filename, inscription ins)
{
    FILE *f = fopen(filename, "a");
    if(f != NULL)
    {
        fprintf(f, "%d;%d;%d;%s\n",
                ins.id_inscription, 
                ins.id_centre, 
                ins.id_coach,
                ins.date);
        fclose(f);
        return 1;
    }
    else 
        return 0;
}

int verifierInscriptionExiste(char *filename, int id_centre, int id_coach, char *date)
{
    FILE *f = fopen(filename, "r");
    if(f != NULL)
    {
        inscription ins;
        char ligne[256];
        while(fgets(ligne, sizeof(ligne), f))
        {
            if(sscanf(ligne, "%d;%d;%d;%10[^\n]", 
                     &ins.id_inscription, &ins.id_centre, &ins.id_coach, ins.date) == 4)
            {
                if(ins.id_centre == id_centre && ins.id_coach == id_coach && strcmp(ins.date, date) == 0)
                {
                    fclose(f);
                    return 1; // Inscription existe déjà
                }
            }
        }
        fclose(f);
    }
    return 0; // Inscription n'existe pas
}

int genererIdInscription() {
    int dernier_id = 0;
    FILE *f = fopen("inscri.txt", "r");
    
    if(f != NULL)
    {
        inscription ins;
        char ligne[256];
        while(fgets(ligne, sizeof(ligne), f))
        {
            if(sscanf(ligne, "%d;", &ins.id_inscription) == 1)
            {
                if(ins.id_inscription > dernier_id)
                {
                    dernier_id = ins.id_inscription;
                }
            }
        }
        fclose(f);
    }
    
    return dernier_id + 1;
}

void afficherInscriptions(char *filename) {
    FILE *f = fopen(filename, "r");
    if(f != NULL)
    {
        inscription ins;
        printf("Liste des inscriptions:\n");
        printf("ID\tCentre\tCoach\tDate\n");
        printf("--------------------------------\n");
        
        while(fscanf(f, "%d;%d;%d;%10[^\n]\n", 
                    &ins.id_inscription, &ins.id_centre, &ins.id_coach, ins.date) != EOF)
        {
            printf("%d\t%d\t%d\t%s\n", 
                   ins.id_inscription, ins.id_centre, ins.id_coach, ins.date);
        }
        fclose(f);
    }
}
//entraineurs

Entraineur chercher_entraineur(const char *id) {
    Entraineur e;
    memset(&e, 0, sizeof(Entraineur));
    
    FILE *f = fopen("entraineurs.txt", "r");
    char ligne[500];
   
    if (f != NULL) {
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) > 0) {
                char *token;
                int field = 0;
                char ligne_copy[500];
                strcpy(ligne_copy, ligne);
                
                token = strtok(ligne_copy, "|");
                while (token != NULL && field < 9) {  // CHANGÉ: 9 champs au lieu de 10
                    switch(field) {
                        case 0: strncpy(e.id, token, sizeof(e.id)-1); break;
                        case 1: strncpy(e.nom_complet, token, sizeof(e.nom_complet)-1); break;  // CHANGÉ
                        case 2: strncpy(e.specialite, token, sizeof(e.specialite)-1); break;
                        case 3: strncpy(e.email, token, sizeof(e.email)-1); break;
                        case 4: strncpy(e.experience, token, sizeof(e.experience)-1); break;
                        case 5: strncpy(e.reseaux_sociaux, token, sizeof(e.reseaux_sociaux)-1); break;
                        case 6: strncpy(e.statut, token, sizeof(e.statut)-1); break;
                        case 7: strncpy(e.jours, token, sizeof(e.jours)-1); break;
                        case 8: strncpy(e.disponibilite, token, sizeof(e.disponibilite)-1); break;
                    }
                    field++;
                    token = strtok(NULL, "|");
                }
                
                if (strcmp(e.id, id) == 0) {
                    fclose(f);
                    return e;
                }
            }
            memset(&e, 0, sizeof(Entraineur));
        }
        fclose(f);
    }
    
    strcpy(e.id, "Non trouvé");
    return e;
}
void ajouter_entraineur(Entraineur e) {
    FILE *f = fopen("entraineurs.txt", "a");
    if (f != NULL) {
        // CHANGÉ: 9 champs au lieu de 10
        fprintf(f, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n",
                e.id, e.nom_complet, e.specialite, e.email,
                e.experience, e.reseaux_sociaux, e.statut, e.jours, e.disponibilite);
        fclose(f);
    } else {
        f = fopen("entraineurs.txt", "w");
        if (f != NULL) {
            fprintf(f, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n",
                    e.id, e.nom_complet, e.specialite, e.email,
                    e.experience, e.reseaux_sociaux, e.statut, e.jours, e.disponibilite);
            fclose(f);
        }
    }
}

void modifier_entraineur(const char *id, Entraineur nouvelles_info) {
    FILE *f = fopen("entraineurs.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char ligne[500];
    Entraineur e;
    int trouve = 0;
   
    if (f != NULL && temp != NULL) {
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) > 0) {
                memset(&e, 0, sizeof(Entraineur));
                char *token;
                int field = 0;
                char ligne_copy[500];
                strcpy(ligne_copy, ligne);
                
                token = strtok(ligne_copy, "|");
                while (token != NULL && field < 9) {  // CHANGÉ: 9 champs
                    if (field == 0) strncpy(e.id, token, sizeof(e.id)-1);
                    field++;
                    token = strtok(NULL, "|");
                }
                
                if (strcmp(e.id, id) == 0) {
                    // CHANGÉ: 9 champs
                    fprintf(temp, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n",
                            nouvelles_info.id, nouvelles_info.nom_complet,
                            nouvelles_info.specialite, nouvelles_info.email,
                            nouvelles_info.experience, nouvelles_info.reseaux_sociaux,
                            nouvelles_info.statut, nouvelles_info.jours, nouvelles_info.disponibilite);
                    trouve = 1;
                } else {
                    fprintf(temp, "%s\n", ligne);
                }
            }
        }
       
        fclose(f);
        fclose(temp);
       
        if (trouve) {
            remove("entraineurs.txt");
            rename("temp.txt", "entraineurs.txt");
        } else {
            remove("temp.txt");
        }
    } else {
        if (f) fclose(f);
        if (temp) {
            fclose(temp);
            remove("temp.txt");
        }
    }
}
void supprimer_entraineur(const char *id) {
    FILE *f = fopen("entraineurs.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char ligne[500];
    Entraineur e;
    int trouve = 0;
   
    if (f != NULL && temp != NULL) {
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) > 0) {
                memset(&e, 0, sizeof(Entraineur));
                char *token;
                int field = 0;
                char ligne_copy[500];
                strcpy(ligne_copy, ligne);
                
                token = strtok(ligne_copy, "|");
                while (token != NULL && field < 10) {
                    if (field == 0) strncpy(e.id, token, sizeof(e.id)-1);
                    field++;
                    token = strtok(NULL, "|");
                }
                
                if (strcmp(e.id, id) != 0) {
                    fprintf(temp, "%s\n", ligne);
                } else {
                    trouve = 1;
                }
            }
        }
       
        fclose(f);
        fclose(temp);
       
        if (trouve) {
            remove("entraineurs.txt");
            rename("temp.txt", "entraineurs.txt");
        } else {
            remove("temp.txt");
        }
    } else {
        if (f) fclose(f);
        if (temp) {
            fclose(temp);
            remove("temp.txt");
        }
    }
}

void vider_treeview(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) return;
    
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    if (model && GTK_IS_LIST_STORE(model)) {
        gtk_list_store_clear(GTK_LIST_STORE(model));
    }
}

void configurer_columns_treeview(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) return;
    
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    GList *iter = columns;
    while (iter) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_COLUMN(iter->data));
        iter = iter->next;
    }
    g_list_free(columns);
    
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    // CHANGÉ: 8 colonnes
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);  // CHANGÉ: nom_complet
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Spécialité", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Email", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Expérience", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Réseaux", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Statut", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Jours", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // CHANGÉ: Supprimé la colonne "Disponibilité" pour avoir 8 colonnes
}
void afficher_tous_entraineurs(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) return;
    
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    if (columns == NULL) {
        configurer_columns_treeview(treeview);
    }
    if (columns) g_list_free(columns);
    
    GtkTreeIter iter;
    GtkListStore *store;
    FILE *f;
    char ligne[500];
   
    // CHANGÉ: 8 colonnes au lieu de 9 (pas de pronom)
    store = gtk_list_store_new(8,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_STRING);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
   
    f = fopen("entraineurs.txt", "r");
    if (f != NULL) {
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) > 0) {
                Entraineur e;
                memset(&e, 0, sizeof(Entraineur));
                
                char *token;
                int field = 0;
                char ligne_copy[500];
                strcpy(ligne_copy, ligne);
                
                token = strtok(ligne_copy, "|");
                while (token != NULL && field < 9) {  // CHANGÉ: 9 champs
                    switch(field) {
                        case 0: strncpy(e.id, token, sizeof(e.id)-1); break;
                        case 1: strncpy(e.nom_complet, token, sizeof(e.nom_complet)-1); break;  // CHANGÉ
                        case 2: strncpy(e.specialite, token, sizeof(e.specialite)-1); break;
                        case 3: strncpy(e.email, token, sizeof(e.email)-1); break;
                        case 4: strncpy(e.experience, token, sizeof(e.experience)-1); break;
                        case 5: strncpy(e.reseaux_sociaux, token, sizeof(e.reseaux_sociaux)-1); break;
                        case 6: strncpy(e.statut, token, sizeof(e.statut)-1); break;
                        case 7: strncpy(e.jours, token, sizeof(e.jours)-1); break;
                        case 8: strncpy(e.disponibilite, token, sizeof(e.disponibilite)-1); break;
                    }
                    field++;
                    token = strtok(NULL, "|");
                }
                
                if (strlen(e.id) > 0) {
                    gtk_list_store_append(store, &iter);
                    // CHANGÉ: nom_complet au lieu de nom
                    gtk_list_store_set(store, &iter,
                        0, e.id,
                        1, e.nom_complet,      // CHANGÉ
                        2, e.specialite,
                        3, e.email,
                        4, e.experience,
                        5, e.reseaux_sociaux,
                        6, e.statut,
                        7, e.jours,
                        // CHANGÉ: 8 colonnes seulement
                        -1);
                }
            }
        }
        fclose(f);
    } else {
        f = fopen("entraineurs.txt", "w");
        if (f) fclose(f);
    }
}

void enregistrer_inscription(Inscription i) {
    FILE *f = fopen("inscriptions.txt", "a");
    if (f != NULL) {
        fprintf(f, "%s|%s|%s|%s|%s|%s\n",
                i.nom_complet, i.identifiant, i.cours,
                i.horaire, i.prix, i.salle);
        fclose(f);
    } else {
        f = fopen("inscriptions.txt", "w");
        if (f != NULL) {
            fprintf(f, "%s|%s|%s|%s|%s|%s\n",
                    i.nom_complet, i.identifiant, i.cours,
                    i.horaire, i.prix, i.salle);
            fclose(f);
        }
    }
}


#define FICHIER_EMPLOI "emploi_entraineurs.txt"
#define FICHIER_PROGRESSION "progression_clients.txt"

// Structure pour les données du profil
typedef struct {
    char entraineur_id[20];
    char emploi_lundi[50];
    char emploi_mardi[50];
    char emploi_mercredi[50];
    char emploi_jeudi[50];
    char emploi_vendredi[50];
    char emploi_samedi[50];
} EmploiEntraineur;

typedef struct {
    char entraineur_id[20];
    char client_id[20];
    char semaine1[100];
    char semaine2[100];
    char semaine3[100];
    char semaine4[100];
} ProgressionClient;

// Structure pour les spécialités

// ========== FONCTIONS DE BASE ==========
void parser_specialites(const char *specialite_str, Specialites *spec) {
    memset(spec, 0, sizeof(Specialites));
    
    char spec_copy[100];
    strcpy(spec_copy, specialite_str);
    char *token = strtok(spec_copy, ",");
    
    while (token) {
        char *clean_token = token;
        while (*clean_token == ' ') clean_token++;
        
        if (strstr(clean_token, "cardio") || strcmp(clean_token, "cardio") == 0)
            spec->cardio = 1;
        else if (strstr(clean_token, "dance") || strcmp(clean_token, "dance") == 0)
            spec->dance = 1;
        else if (strstr(clean_token, "musculation") || strcmp(clean_token, "musculation") == 0)
            spec->musculation = 1;
        else if (strstr(clean_token, "boxe") || strcmp(clean_token, "boxe") == 0)
            spec->boxe = 1;
        else if (strstr(clean_token, "gymnastique") || strcmp(clean_token, "gymnastique") == 0)
            spec->gymnastique = 1;
        
        token = strtok(NULL, ",");
    }
}

int correspond_specialites(const char *objectifs_str, const Specialites *spec_entraineur) {
    int obj_cardio, obj_dance, obj_musculation, obj_boxe, obj_gymnastique;
    
    if (sscanf(objectifs_str, "%d,%d,%d,%d,%d", 
               &obj_cardio, &obj_dance, &obj_musculation, &obj_boxe, &obj_gymnastique) != 5) {
        return 0;
    }
    
    return (obj_cardio && spec_entraineur->cardio) ||
           (obj_dance && spec_entraineur->dance) ||
           (obj_musculation && spec_entraineur->musculation) ||
           (obj_boxe && spec_entraineur->boxe) ||
           (obj_gymnastique && spec_entraineur->gymnastique);
}

// ========== FONCTIONS POUR CLIENTS ==========
void configurer_columns_profile_client(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) return;
    
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    GList *iter = columns;
    while (iter) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_COLUMN(iter->data));
        iter = iter->next;
    }
    if (columns) g_list_free(columns);
    
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    // ID Membre
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID Membre", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_min_width(column, 80);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Poids (éditable)
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    column = gtk_tree_view_column_new_with_attributes("Poids (kg)", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_min_width(column, 80);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Progression Semaine 1 (éditable)
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    column = gtk_tree_view_column_new_with_attributes("Semaine 1", renderer, "text", 2, NULL);
    gtk_tree_view_column_set_min_width(column, 100);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Progression Semaine 2 (éditable)
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    column = gtk_tree_view_column_new_with_attributes("Semaine 2", renderer, "text", 3, NULL);
    gtk_tree_view_column_set_min_width(column, 100);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Progression Semaine 3 (éditable)
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    column = gtk_tree_view_column_new_with_attributes("Semaine 3", renderer, "text", 4, NULL);
    gtk_tree_view_column_set_min_width(column, 100);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Progression Semaine 4 (éditable)
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    column = gtk_tree_view_column_new_with_attributes("Semaine 4", renderer, "text", 5, NULL);
    gtk_tree_view_column_set_min_width(column, 100);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Améliorer l'apparence
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview), TRUE);
}
void charger_progression_existante(const char *entraineur_id, const char *client_id,
                                   char *sem1, char *sem2, char *sem3, char *sem4) {
    // Initialiser avec valeurs par défaut
    strcpy(sem1, "À remplir");
    strcpy(sem2, "À remplir");
    strcpy(sem3, "À remplir");
    strcpy(sem4, "À remplir");
    
    FILE *f = fopen("progression_clients.txt", "r");
    if (!f) {
        g_print("DEBUG: Fichier progression_clients.txt non trouvé\n");
        return;
    }
    
    char ligne[300];
    int trouve = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) > 0) {
            char eid[50], cid[50], s1[100], s2[100], s3[100], s4[100];
            
            // Parser la ligne
            if (sscanf(ligne, "%49[^|]|%49[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]", 
                      eid, cid, s1, s2, s3, s4) == 6) {
                
                // Vérifier si c'est la bonne combinaison entraîneur-client
                if (strcmp(eid, entraineur_id) == 0 && strcmp(cid, client_id) == 0) {
                    strcpy(sem1, s1);
                    strcpy(sem2, s2);
                    strcpy(sem3, s3);
                    strcpy(sem4, s4);
                    trouve = 1;
                    g_print("DEBUG: Progression trouvée pour client %s\n", client_id);
                    break;
                }
            }
        }
    }
    
    fclose(f);
    
    if (!trouve) {
        g_print("DEBUG: Aucune progression trouvée pour client %s\n", client_id);
    }
}
void charger_clients_entraineur(GtkWidget *treeview, const char *entraineur_id) {
    if (!treeview || !entraineur_id) return;
    
    g_print("\n=== CHARGEMENT CLIENTS POUR ENTRAÎNEUR %s ===\n", entraineur_id);
    
    // 1. Récupérer l'entraîneur
    Entraineur e = chercher_entraineur(entraineur_id);
    if (strcmp(e.id, "Non trouvé") == 0) {
        g_print("ERROR: Entraîneur non trouvé\n");
        return;
    }
    
    g_print("Entraîneur: %s (ID: %s)\n", e.nom_complet, e.id);
    g_print("Spécialités: %s\n", e.specialite);
    
    // 2. Parser les spécialités de l'entraîneur
    Specialites spec_entraineur;
    parser_specialites(e.specialite, &spec_entraineur);
    
    g_print("Spécialités parsées: cardio=%d, dance=%d, musculation=%d, boxe=%d, gymnastique=%d\n",
           spec_entraineur.cardio, spec_entraineur.dance, spec_entraineur.musculation,
           spec_entraineur.boxe, spec_entraineur.gymnastique);
    
    // 3. Configurer les colonnes du TreeView
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    if (columns == NULL) {
        configurer_columns_profile_client(treeview);
    }
    if (columns) g_list_free(columns);
    
    // 4. Créer le store avec 6 colonnes
    GtkListStore *store = gtk_list_store_new(6, 
        G_TYPE_STRING,  // ID Membre
        G_TYPE_STRING,  // Poids (kg)
        G_TYPE_STRING,  // Semaine 1
        G_TYPE_STRING,  // Semaine 2
        G_TYPE_STRING,  // Semaine 3
        G_TYPE_STRING); // Semaine 4
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    
    // 5. Ouvrir le fichier reservation.txt
    FILE *f = fopen("reservation.txt", "r");
    if (!f) {
        g_print("ERROR: Fichier reservation.txt non trouvé\n");
        // Créer un fichier de test
        f = fopen("reservation.txt", "w");
        if (f) {
            fprintf(f, "Membre:22 | Objectifs:1,1,1,1,1 | Disponibilite:Matin | Jours:1,1,1,1,0\n");
            fprintf(f, "Membre:23 | Objectifs:1,0,0,0,0 | Disponibilite:Après-midi | Jours:0,1,0,1,0\n");
            fprintf(f, "Membre:24 | Objectifs:0,1,0,0,0 | Disponibilite:Soir | Jours:1,0,1,0,1\n");
            fprintf(f, "Membre:25 | Objectifs:0,0,1,0,0 | Disponibilite:Toute la journée | Jours:1,1,0,0,0\n");
            fclose(f);
            f = fopen("reservation.txt", "r");
        }
    }
    
    int nb_clients = 0;
    if (f) {
        char ligne[300];
        GtkTreeIter iter;
        
        g_print("\n=== LECTURE FICHIER RESERVATION ===\n");
        
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) == 0) continue;
            
            g_print("Ligne brute: %s\n", ligne);
            
            // Variables pour stocker les données
            char membre_id[50] = "";
            char objectifs[50] = "";
            char disponibilite[50] = "";
            char jours[50] = "";
            
            // Parser la ligne avec le format exact
            // Format: "Membre:22 | Objectifs:1,1,1,1,1 | Disponibilite:Matin | Jours:1,1,1,1,0"
            
            char *token = strtok(ligne, "|");
            while (token) {
                // Nettoyer les espaces
                char *clean_token = token;
                while (*clean_token == ' ') clean_token++;
                
                g_print("Token nettoyé: '%s'\n", clean_token);
                
                if (strstr(clean_token, "Membre:")) {
                    strcpy(membre_id, clean_token + 7); // Après "Membre:"
                    g_print("  -> ID Membre: %s\n", membre_id);
                } 
                else if (strstr(clean_token, "Objectifs:")) {
                    strcpy(objectifs, clean_token + 10); // Après "Objectifs:"
                    g_print("  -> Objectifs: %s\n", objectifs);
                } 
                else if (strstr(clean_token, "Disponibilite:")) {
                    strcpy(disponibilite, clean_token + 14); // Après "Disponibilite:"
                    g_print("  -> Disponibilité: %s\n", disponibilite);
                } 
                else if (strstr(clean_token, "Jours:")) {
                    strcpy(jours, clean_token + 6); // Après "Jours:"
                    g_print("  -> Jours: %s\n", jours);
                }
                
                token = strtok(NULL, "|");
            }
            
            // Vérifier si les objectifs correspondent aux spécialités de l'entraîneur
            if (strlen(objectifs) > 0) {
                g_print("Vérification correspondance pour Membre %s...\n", membre_id);
                
                int correspond = correspond_specialites(objectifs, &spec_entraineur);
                
                if (correspond) {
                    g_print("  -> CORRESPOND ! Ajout au TreeView\n");
                    
                    // Charger la progression existante
                    char sem1[100], sem2[100], sem3[100], sem4[100];
                    charger_progression_existante(entraineur_id, membre_id, sem1, sem2, sem3, sem4);
                    
                    // Ajouter au TreeView
                    gtk_list_store_append(store, &iter);
                    gtk_list_store_set(store, &iter,
                        0, membre_id,
                        1, "70",  // Poids par défaut
                        2, (strlen(sem1) > 0 && strcmp(sem1, "Non défini") != 0) ? sem1 : "À remplir",
                        3, (strlen(sem2) > 0 && strcmp(sem2, "Non défini") != 0) ? sem2 : "À remplir",
                        4, (strlen(sem3) > 0 && strcmp(sem3, "Non défini") != 0) ? sem3 : "À remplir",
                        5, (strlen(sem4) > 0 && strcmp(sem4, "Non défini") != 0) ? sem4 : "À remplir",
                        -1);
                    
                    nb_clients++;
                    g_print("  -> Client ajouté (total: %d)\n", nb_clients);
                } else {
                    g_print("  -> NE CORRESPOND PAS\n");
                }
            }
            g_print("---\n");
        }
        
        fclose(f);
        
        g_print("\n=== RÉSULTAT ===\n");
        g_print("Total clients chargés: %d\n", nb_clients);
        
        // Si aucun client, afficher un message
        if (nb_clients == 0) {
            g_print("Aucun client correspondant aux spécialités\n");
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                0, "Aucun client",
                1, "",
                2, "Vérifiez les spécialités",
                3, "",
                4, "",
                5, "",
                -1);
        }
        
    } else {
        g_print("ERROR: Impossible d'ouvrir reservation.txt\n");
    }
    
    g_object_unref(store);
    g_print("=== FIN CHARGEMENT CLIENTS ===\n\n");
}
void sauvegarder_progression_clients(GtkWidget *treeview, const char *entraineur_id) {
    if (!treeview || !entraineur_id) return;
    
    g_print("\n=== SAUVEGARDE PROGRESSION CLIENTS POUR ENTRAÎNEUR %s ===\n", entraineur_id);
    
    // 1. Lire les progressions existantes
    FILE *f_in = fopen("progression_clients.txt", "r");
    FILE *f_temp = fopen("temp_progression.txt", "w");
    
    if (!f_temp) {
        g_print("ERROR: Impossible de créer fichier temporaire\n");
        if (f_in) fclose(f_in);
        return;
    }
    
    // 2. Récupérer les données du TreeView
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(model, &iter);
    
    int nb_sauvegardes = 0;
    
    while (valid) {
        gchar *client_id = NULL;
        gchar *poids = NULL;
        gchar *sem1 = NULL;
        gchar *sem2 = NULL;
        gchar *sem3 = NULL;
        gchar *sem4 = NULL;
        
        gtk_tree_model_get(model, &iter, 
            0, &client_id,
            1, &poids,
            2, &sem1,
            3, &sem2,
            4, &sem3,
            5, &sem4,
            -1);
        
        // Vérifier que ce n'est pas la ligne "Aucun client"
        if (client_id && strcmp(client_id, "Aucun client") != 0) {
            // Écrire la nouvelle ligne
            fprintf(f_temp, "%s|%s|%s|%s|%s|%s\n",
                    entraineur_id,
                    client_id ? client_id : "",
                    sem1 ? sem1 : "À remplir",
                    sem2 ? sem2 : "À remplir",
                    sem3 ? sem3 : "À remplir",
                    sem4 ? sem4 : "À remplir");
            
            g_print("Sauvegardé: Client %s - S1:%s S2:%s S3:%s S4:%s\n",
                   client_id ? client_id : "N/A",
                   sem1 ? sem1 : "N/A",
                   sem2 ? sem2 : "N/A",
                   sem3 ? sem3 : "N/A",
                   sem4 ? sem4 : "N/A");
            
            nb_sauvegardes++;
        }
        
        // Libérer la mémoire
        if (client_id) g_free(client_id);
        if (poids) g_free(poids);
        if (sem1) g_free(sem1);
        if (sem2) g_free(sem2);
        if (sem3) g_free(sem3);
        if (sem4) g_free(sem4);
        
        valid = gtk_tree_model_iter_next(model, &iter);
    }
    
    g_print("Total lignes sauvegardées: %d\n", nb_sauvegardes);
    
    // 3. Copier les autres lignes (autres entraîneurs)
    if (f_in) {
        char ligne[300];
        while (fgets(ligne, sizeof(ligne), f_in)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) > 0) {
                char eid[50], cid[50];
                // Lire l'ID entraîneur et client
                if (sscanf(ligne, "%49[^|]|%49[^|]", eid, cid) == 2) {
                    // Si c'est pour un autre entraîneur, le garder
                    if (strcmp(eid, entraineur_id) != 0) {
                        fprintf(f_temp, "%s\n", ligne);
                    }
                }
            }
        }
        fclose(f_in);
    }
    
    fclose(f_temp);
    
    // 4. Remplacer l'ancien fichier
    remove("progression_clients.txt");
    rename("temp_progression.txt", "progression_clients.txt");
    
    g_print("=== SAUVEGARDE TERMINÉE ===\n\n");
}

// ========== FONCTIONS POUR EMPLOI DU TEMPS ==========
void configurer_columns_profile_emploi(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) return;
    
    // Supprimer les colonnes existantes
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    GList *iter_col = columns;
    while (iter_col) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_COLUMN(iter_col->data));
        iter_col = iter_col->next;
    }
    if (columns) g_list_free(columns);
    
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    // Colonne 1: Jour
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Jour", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_min_width(column, 80);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Colonne 2: Matin (CHECKBUTTON)
    renderer = gtk_cell_renderer_toggle_new();
    column = gtk_tree_view_column_new_with_attributes("Matin", renderer, "active", 1, NULL);
    gtk_tree_view_column_set_min_width(column, 60);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Colonne 3: Après-midi (CHECKBUTTON)
    renderer = gtk_cell_renderer_toggle_new();
    column = gtk_tree_view_column_new_with_attributes("Après-midi", renderer, "active", 2, NULL);
    gtk_tree_view_column_set_min_width(column, 80);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Colonne 4: Soir (CHECKBUTTON)
    renderer = gtk_cell_renderer_toggle_new();
    column = gtk_tree_view_column_new_with_attributes("Soir", renderer, "active", 3, NULL);
    gtk_tree_view_column_set_min_width(column, 60);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Colonne 5: Toute journée (CHECKBUTTON)
    renderer = gtk_cell_renderer_toggle_new();
    column = gtk_tree_view_column_new_with_attributes("Toute journée", renderer, "active", 4, NULL);
    gtk_tree_view_column_set_min_width(column, 100);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Améliorer l'apparence
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview), TRUE);
}
void charger_emploi_existant(const char *entraineur_id, char jours[6][5]) {
    // Initialiser avec "-"
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            jours[i][j] = '-';
        }
        jours[i][4] = '\0';
    }
    
    FILE *f = fopen(FICHIER_EMPLOI, "r");
    if (!f) return;
    
    char ligne[200];
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) > 0) {
            char eid[50], jour[20], matin[10], am[10], soir[10], tj[10];
            
            if (sscanf(ligne, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]", 
                      eid, jour, matin, am, soir, tj) == 6) {
                if (strcmp(eid, entraineur_id) == 0) {
                    int idx = -1;
                    if (strcmp(jour, "Lundi") == 0) idx = 0;
                    else if (strcmp(jour, "Mardi") == 0) idx = 1;
                    else if (strcmp(jour, "Mercredi") == 0) idx = 2;
                    else if (strcmp(jour, "Jeudi") == 0) idx = 3;
                    else if (strcmp(jour, "Vendredi") == 0) idx = 4;
                    else if (strcmp(jour, "Samedi") == 0) idx = 5;
                    
                    if (idx >= 0) {
                        jours[idx][0] = matin[0];
                        jours[idx][1] = am[0];
                        jours[idx][2] = soir[0];
                        jours[idx][3] = tj[0];
                    }
                }
            }
        }
    }
    
    fclose(f);
}

void charger_emploi_entraineur(GtkWidget *treeview, const char *entraineur_id) {
    if (!treeview) return;
    
    g_print("DEBUG: Chargement emploi (checkbuttons) pour entraîneur: %s\n", entraineur_id);
    
    // Configurer les colonnes
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    if (columns == NULL) {
        configurer_columns_profile_emploi(treeview);
    }
    if (columns) g_list_free(columns);
    
    // Créer le store avec G_TYPE_BOOLEAN pour les checkbuttons
    GtkListStore *store = gtk_list_store_new(5,
        G_TYPE_STRING,    // Jour
        G_TYPE_BOOLEAN,   // Matin (TRUE/FALSE)
        G_TYPE_BOOLEAN,   // Après-midi (TRUE/FALSE)
        G_TYPE_BOOLEAN,   // Soir (TRUE/FALSE)
        G_TYPE_BOOLEAN);  // Toute journée (TRUE/FALSE)
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    
    const char *jours[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
    
    // Vérifier les jours depuis emploi_entraineurs.txt d'abord
    gboolean jours_actifs[6][4] = {
        {FALSE, FALSE, FALSE, FALSE}, // Lundi
        {FALSE, FALSE, FALSE, FALSE}, // Mardi
        {FALSE, FALSE, FALSE, FALSE}, // Mercredi
        {FALSE, FALSE, FALSE, FALSE}, // Jeudi
        {FALSE, FALSE, FALSE, FALSE}, // Vendredi
        {FALSE, FALSE, FALSE, FALSE}  // Samedi
    };
    
    // Charger depuis le fichier si existe
    FILE *f = fopen("emploi_entraineurs.txt", "r");
    if (f) {
        char ligne[200];
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) > 0) {
                char eid[50], jour[20], matin[10], aprem[10], soir[10], tj[10];
                
                if (sscanf(ligne, "%49[^|]|%19[^|]|%9[^|]|%9[^|]|%9[^|]|%9[^|]", 
                          eid, jour, matin, aprem, soir, tj) == 6) {
                    
                    if (strcmp(eid, entraineur_id) == 0) {
                        int idx = -1;
                        if (strcmp(jour, "Lundi") == 0) idx = 0;
                        else if (strcmp(jour, "Mardi") == 0) idx = 1;
                        else if (strcmp(jour, "Mercredi") == 0) idx = 2;
                        else if (strcmp(jour, "Jeudi") == 0) idx = 3;
                        else if (strcmp(jour, "Vendredi") == 0) idx = 4;
                        else if (strcmp(jour, "Samedi") == 0) idx = 5;
                        
                        if (idx >= 0) {
                            jours_actifs[idx][0] = (strcmp(matin, "OUI") == 0 || strcmp(matin, "1") == 0) ? TRUE : FALSE;
                            jours_actifs[idx][1] = (strcmp(aprem, "OUI") == 0 || strcmp(aprem, "1") == 0) ? TRUE : FALSE;
                            jours_actifs[idx][2] = (strcmp(soir, "OUI") == 0 || strcmp(soir, "1") == 0) ? TRUE : FALSE;
                            jours_actifs[idx][3] = (strcmp(tj, "OUI") == 0 || strcmp(tj, "1") == 0) ? TRUE : FALSE;
                        }
                    }
                }
            }
        }
        fclose(f);
        g_print("DEBUG: Chargé depuis emploi_entraineurs.txt\n");
    } else {
        // Si pas de fichier, charger depuis les jours de l'entraîneur
        g_print("DEBUG: Fichier emploi_entraineurs.txt non trouvé, utilisation entraineurs.txt\n");
        
        Entraineur e = chercher_entraineur(entraineur_id);
        if (strcmp(e.id, "Non trouvé") != 0 && strlen(e.jours) > 0) {
            g_print("DEBUG: Jours de l'entraîneur: %s\n", e.jours);
            
            char jours_copy[100];
            strcpy(jours_copy, e.jours);
            char *token = strtok(jours_copy, ",");
            
            while (token) {
                char *clean_token = token;
                while (*clean_token == ' ') clean_token++;
                
                for (int i = 0; i < 6; i++) {
                    if (strcasecmp(clean_token, jours[i]) == 0) {
                        // Cocher "Toute la journée" par défaut
                        jours_actifs[i][3] = TRUE;
                        g_print("DEBUG: Jour actif trouvé: %s\n", jours[i]);
                        break;
                    }
                }
                token = strtok(NULL, ",");
            }
        }
    }
    
    // Ajouter les lignes au TreeView
    GtkTreeIter iter;
    for (int i = 0; i < 6; i++) {
        gtk_list_store_append(store, &iter);
        
        gtk_list_store_set(store, &iter,
            0, jours[i],
            1, jours_actifs[i][0],  // Matin
            2, jours_actifs[i][1],  // Après-midi
            3, jours_actifs[i][2],  // Soir
            4, jours_actifs[i][3],  // Toute journée
            -1);
    }
    
    g_object_unref(store);
    g_print("DEBUG: Emploi chargé avec checkbuttons\n");
}
void sauvegarder_emploi_entraineur(GtkWidget *treeview, const char *entraineur_id) {
    if (!treeview || !entraineur_id) return;
    
    g_print("DEBUG: Sauvegarde emploi (checkbuttons) pour entraîneur: %s\n", entraineur_id);
    
    // Lire le fichier existant
    FILE *f_in = fopen("emploi_entraineurs.txt", "r");
    FILE *f_temp = fopen("temp_emploi.txt", "w");
    
    if (!f_temp) {
        g_print("ERROR: Impossible de créer fichier temporaire\n");
        if (f_in) fclose(f_in);
        return;
    }
    
    const char *jours[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(model, &iter);
    int idx = 0;
    
    // Écrire les nouvelles données
    while (valid && idx < 6) {
        gboolean matin, aprem, soir, toute_journee;
        
        gtk_tree_model_get(model, &iter,
            1, &matin,
            2, &aprem,
            3, &soir,
            4, &toute_journee,
            -1);
        
        // Convertir TRUE/FALSE en OUI/NON
        const char *matin_str = matin ? "OUI" : "NON";
        const char *aprem_str = aprem ? "OUI" : "NON";
        const char *soir_str = soir ? "OUI" : "NON";
        const char *tj_str = toute_journee ? "OUI" : "NON";
        
        // Écrire la ligne
        fprintf(f_temp, "%s|%s|%s|%s|%s|%s\n",
                entraineur_id, 
                jours[idx],
                matin_str,
                aprem_str,
                soir_str,
                tj_str);
        
        g_print("DEBUG: Sauvegardé %s: M=%s A=%s S=%s T=%s\n",
                jours[idx], matin_str, aprem_str, soir_str, tj_str);
        
        idx++;
        valid = gtk_tree_model_iter_next(model, &iter);
    }
    
    // Copier les autres entraîneurs
    if (f_in) {
        char ligne[200];
        while (fgets(ligne, sizeof(ligne), f_in)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) > 0) {
                char eid[50];
                if (sscanf(ligne, "%49[^|]", eid) == 1) {
                    if (strcmp(eid, entraineur_id) != 0) {
                        fprintf(f_temp, "%s\n", ligne);
                    }
                }
            }
        }
        fclose(f_in);
    }
    
    fclose(f_temp);
    
    // Remplacer l'ancien fichier
    remove("emploi_entraineurs.txt");
    rename("temp_emploi.txt", "emploi_entraineurs.txt");
    
    g_print("DEBUG: Emploi sauvegardé avec succès\n");
}
// ========== FONCTION PRINCIPALE ==========
void afficher_profile_entraineur(const char *nom, const char *id) {
    GtkWidget *window = create_Profil_d_un_entra__neur();
    
    if (window) {
        // Remplir les champs
        GtkWidget *entry_nom = lookup_widget(window, "entry44");
        GtkWidget *entry_id = lookup_widget(window, "entry45");
        
        if (entry_nom) gtk_entry_set_text(GTK_ENTRY(entry_nom), nom);
        if (entry_id) gtk_entry_set_text(GTK_ENTRY(entry_id), id);
        
        // Charger les clients
        GtkWidget *treeview_clients = lookup_widget(window, "treeview_list_client3");
        if (treeview_clients) {
            charger_clients_entraineur(treeview_clients, id);
        }
        
        // Charger l'emploi du temps
        GtkWidget *treeview_emploi = lookup_widget(window, "treeview_emploit4");
        if (treeview_emploi) {
            charger_emploi_entraineur(treeview_emploi, id);
        }
        
        // Connecter les boutons
        GtkWidget *btn_valider_clients = lookup_widget(window, "button_valider_client100");
        GtkWidget *btn_valider_emploi = lookup_widget(window, "button_valider_emplois101");
        
        if (btn_valider_clients) {
            g_signal_connect(btn_valider_clients, "clicked",
                            G_CALLBACK(on_button_valider_client100_clicked),
                            NULL);
        }
        if (btn_valider_emploi) {
            g_signal_connect(btn_valider_emploi, "clicked",
                            G_CALLBACK(on_button_valider_emplois101_clicked),
                            NULL);
        }
        
        gtk_widget_show(window);
    }
}
//gestion du cours
// ==================== FONCTIONS COURS ====================

void ajouter_cours(Cours c) {
    FILE *f = fopen("cours.txt", "a");
    if (f != NULL) {
        fprintf(f, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s\n",
                c.id, c.nom, c.type, c.niveau, c.jours,
                c.heure, c.minute, c.periode, c.coach, c.equipement);
        fclose(f);
        printf("Cours ajouté: %s - %s\n", c.id, c.nom);
    } else {
        printf("Erreur: Impossible d'ouvrir cours.txt en écriture\n");
    }
}

int supprimer_cours(char *id) {
    FILE *f = fopen("cours.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    int trouve = 0;
    char ligne[256];
    
    if (f == NULL || temp == NULL) {
        printf("Erreur: Impossible d'ouvrir les fichiers\n");
        if (f) fclose(f);
        if (temp) fclose(temp);
        return 0;
    }
    
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) == 0) continue;
        
        char current_id[10];
        sscanf(ligne, "%[^|]", current_id);
        
        if (strcmp(current_id, id) == 0) {
            trouve = 1;
            printf("Cours supprimé: %s\n", id);
        } else {
            fprintf(temp, "%s\n", ligne);
        }
    }
    
    fclose(f);
    fclose(temp);
    
    remove("cours.txt");
    rename("temp.txt", "cours.txt");
    
    return trouve;
}

Cours rechercher_cours(char *id) {
    Cours c;
    FILE *f = fopen("cours.txt", "r");
    char ligne[256];
    
    strcpy(c.id, "");
    
    if (f != NULL) {
        while (fgets(ligne, sizeof(ligne), f) != NULL) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) == 0) continue;
            
            char current_id[10];
            sscanf(ligne, "%[^|]", current_id);
            
            if (strcmp(current_id, id) == 0) {
                sscanf(ligne, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%[^|]|%[^\n]",
                      c.id, c.nom, c.type, c.niveau, c.jours,
                      &c.heure, &c.minute, c.periode, c.coach, c.equipement);
                fclose(f);
                return c;
            }
        }
        fclose(f);
    }
    
    strcpy(c.id, "");
    return c;
}

void modifier_cours(Cours c_modifie) {
    FILE *f = fopen("cours.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char ligne[256];
    
    if (f != NULL && temp != NULL) {
        while (fgets(ligne, sizeof(ligne), f) != NULL) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) == 0) continue;
            
            char current_id[10];
            sscanf(ligne, "%[^|]", current_id);
            
            if (strcmp(current_id, c_modifie.id) == 0) {
                fprintf(temp, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s\n",
                        c_modifie.id, c_modifie.nom, c_modifie.type, 
                        c_modifie.niveau, c_modifie.jours,
                        c_modifie.heure, c_modifie.minute, 
                        c_modifie.periode, c_modifie.coach, 
                        c_modifie.equipement);
            } else {
                fprintf(temp, "%s\n", ligne);
            }
        }
        fclose(f);
        fclose(temp);
        
        remove("cours.txt");
        rename("temp.txt", "cours.txt");
        
        printf("Cours modifié: %s\n", c_modifie.id);
    }
}

int get_tous_cours(Cours liste[]) {
    FILE *f = fopen("cours.txt", "r");
    int count = 0;
    char ligne[256];
    
    if (f == NULL) {
        printf("Erreur: Impossible d'ouvrir cours.txt en lecture\n");
        return 0;
    }
    
    while (fgets(ligne, sizeof(ligne), f) != NULL && count < MAX_COURS) {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) == 0) continue;
        
        Cours c;
        int result = sscanf(ligne, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%[^|]|%[^\n]",
                           c.id, c.nom, c.type, c.niveau, c.jours,
                           &c.heure, &c.minute, c.periode, c.coach, c.equipement);
        
        if (result == 10) {
            liste[count] = c;
            count++;
        }
    }
    
    fclose(f);
    return count;
}

void vider_treeview(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) return;
    
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    if (model != NULL && GTK_IS_LIST_STORE(model)) {
        gtk_list_store_clear(GTK_LIST_STORE(model));
    }
}

void afficher_cours_treeview(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) {
        printf("Erreur: TreeView invalide\n");
        return;
    }
    
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    
    if (store == NULL) {
        store = gtk_list_store_new(10, 
                                   G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
                                   G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                   G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                   G_TYPE_INT);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
        
        // Colonne ID
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        gtk_tree_view_column_set_min_width(column, 50);
        
        // Colonne Nom (éditable)
        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "editable", TRUE, NULL);
        g_signal_connect(renderer, "edited", G_CALLBACK(on_nom_cours_edited), treeview);
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        gtk_tree_view_column_set_min_width(column, 120);
        
        // Colonne Type (éditable)
        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "editable", TRUE, NULL);
        g_signal_connect(renderer, "edited", G_CALLBACK(on_type_cours_edited), treeview);
        column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        // Colonne Niveau (éditable)
        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "editable", TRUE, NULL);
        g_signal_connect(renderer, "edited", G_CALLBACK(on_niveau_cours_edited), treeview);
        column = gtk_tree_view_column_new_with_attributes("Niveau", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        // Colonne Jours
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Jours", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        gtk_tree_view_column_set_min_width(column, 120);
        
        // Colonne Heure
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Heure", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        // Colonne Période
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Période", renderer, "text", 6, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        // Colonne Coach (éditable)
        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "editable", TRUE, NULL);
        g_signal_connect(renderer, "edited", G_CALLBACK(on_coach_cours_edited), treeview);
        column = gtk_tree_view_column_new_with_attributes("Coach", renderer, "text", 7, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        // Colonne Équipement
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Équipement", renderer, "text", 8, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        // Colonne Inscrits
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Inscrits", renderer, "text", 9, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        printf("TreeView initialisé avec colonnes éditable\n");
    } else {
        gtk_list_store_clear(store);
    }
    
    Cours liste_cours[MAX_COURS];
    int nb_cours = get_tous_cours(liste_cours);
    
    printf("Chargement de %d cours dans le TreeView\n", nb_cours);
    
    for (int i = 0; i < nb_cours; i++) {
        char heure_format[10];
        sprintf(heure_format, "%02d:%02d", liste_cours[i].heure, liste_cours[i].minute);
        
        int nb_inscrits = get_nombre_inscriptions(liste_cours[i].id);
        
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                          0, liste_cours[i].id,
                          1, liste_cours[i].nom,
                          2, liste_cours[i].type,
                          3, liste_cours[i].niveau,
                          4, liste_cours[i].jours,
                          5, heure_format,
                          6, liste_cours[i].periode,
                          7, liste_cours[i].coach,
                          8, liste_cours[i].equipement,
                          9, nb_inscrits,
                          -1);
    }
}

void afficher_inscriptions_treeview(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) {
        printf("Erreur: TreeView d'inscriptions invalide\n");
        return;
    }
    
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    
    if (store == NULL) {
        store = gtk_list_store_new(4, 
                                   G_TYPE_STRING,   // 0: ID Membre
                                   G_TYPE_STRING,   // 1: ID Cours
                                   G_TYPE_STRING,   // 2: Date
                                   G_TYPE_STRING);  // 3: Nom du cours
        
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
        
        // Colonne ID Membre
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID Membre", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        gtk_tree_view_column_set_min_width(column, 80);
        
        // Colonne ID Cours
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID Cours", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        gtk_tree_view_column_set_min_width(column, 70);
        
        // Colonne Nom du Cours
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom du Cours", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        gtk_tree_view_column_set_min_width(column, 150);
        
        // Colonne Date
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Date", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        gtk_tree_view_column_set_min_width(column, 100);
        
        printf("TreeView d'inscriptions initialisé\n");
    } else {
        gtk_list_store_clear(store);
    }
    
    // Lire les inscriptions depuis le fichier
    FILE *f = fopen("inscription.txt", "r");
    if (f == NULL) {
        printf("Aucun fichier d'inscription trouvé\n");
        return;
    }
    
    char ligne[100];
    int count = 0;
    
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) == 0) continue;
        
        char id_membre[10], id_cours[10], date[20];
        if (sscanf(ligne, "%[^|]|%[^|]|%[^\n]", id_membre, id_cours, date) == 3) {
            // Rechercher le nom du cours
            Cours c = rechercher_cours(id_cours);
            char nom_cours[50];
            if (strlen(c.id) > 0) {
                strcpy(nom_cours, c.nom);
            } else {
                strcpy(nom_cours, "Cours inconnu");
            }
            
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                              0, id_membre,
                              1, id_cours,
                              2, date,
                              3, nom_cours,
                              -1);
            count++;
        }
    }
    
    fclose(f);
    printf("%d inscriptions chargées dans le TreeView\n", count);
}

void rechercher_et_afficher(GtkWidget *treeview, char *id_recherche) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) return;
    
    GtkListStore *store;
    GtkTreeIter iter;
    
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    if (store == NULL) {
        afficher_cours_treeview(treeview);
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    }
    
    gtk_list_store_clear(store);
    
    if (strlen(id_recherche) == 0) {
        afficher_cours_treeview(treeview);
        return;
    }
    
    Cours c = rechercher_cours(id_recherche);
    if (strlen(c.id) > 0) {
        char heure_format[10];
        sprintf(heure_format, "%02d:%02d", c.heure, c.minute);
        int nb_inscrits = get_nombre_inscriptions(c.id);
        
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                          0, c.id,
                          1, c.nom,
                          2, c.type,
                          3, c.niveau,
                          4, c.jours,
                          5, heure_format,
                          6, c.periode,
                          7, c.coach,
                          8, c.equipement,
                          9, nb_inscrits,
                          -1);
    }
}

int cours_existe(char *id) {
    Cours c = rechercher_cours(id);
    return (strlen(c.id) > 0);
}

// ==================== FONCTIONS INSCRIPTION ====================

void inscrire_membre(Inscription ins) {
    FILE *f = fopen("inscription.txt", "a");
    if (f != NULL) {
        fprintf(f, "%s|%s|%s\n", ins.id_membre, ins.id_cours, ins.date);
        fclose(f);
        printf("Inscription ajoutée: Membre %s -> Cours %s\n", ins.id_membre, ins.id_cours);
    }
}

int est_inscrit(char *id_membre, char *id_cours) {
    FILE *f = fopen("inscription.txt", "r");
    char ligne[100];
    
    if (f != NULL) {
        while (fgets(ligne, sizeof(ligne), f) != NULL) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) == 0) continue;
            
            char id_m[10], id_c[10], date[20];
            sscanf(ligne, "%[^|]|%[^|]|%[^\n]", id_m, id_c, date);
            
            if (strcmp(id_m, id_membre) == 0 && strcmp(id_c, id_cours) == 0) {
                fclose(f);
                return 1;
            }
        }
        fclose(f);
    }
    return 0;
}

int get_nombre_inscriptions(char *id_cours) {
    FILE *f = fopen("inscription.txt", "r");
    int count = 0;
    char ligne[100];
    
    if (f != NULL) {
        while (fgets(ligne, sizeof(ligne), f) != NULL) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) == 0) continue;
            
            char id_m[10], id_c[10], date[20];
            sscanf(ligne, "%[^|]|%[^|]|%[^\n]", id_m, id_c, date);
            
            if (strcmp(id_c, id_cours) == 0) {
                count++;
            }
        }
        fclose(f);
    }
    return count;
}

// ==================== NOUVELLES FONCTIONS ====================

char* generer_id_automatique() {
    static char id[10];
    int max_id = 0;
    
    Cours liste[MAX_COURS];
    int nb_cours = get_tous_cours(liste);
    
    for (int i = 0; i < nb_cours; i++) {
        int current_id = atoi(liste[i].id);
        if (current_id > max_id) {
            max_id = current_id;
        }
    }
    
    max_id++;
    sprintf(id, "%d", max_id);
    return id;
}

int get_nombre_inscrits_total() {
    FILE *f = fopen("inscription.txt", "r");
    int count = 0;
    char ligne[100];
    
    if (f != NULL) {
        while (fgets(ligne, sizeof(ligne), f) != NULL) {
            if (strlen(ligne) > 1) count++;
        }
        fclose(f);
    }
    return count;
}

void charger_coachs_depuis_fichier(GtkWidget *combo) {
    if (!combo || !GTK_IS_COMBO_BOX(combo)) return;
    
    // Essayer de charger depuis coachs.txt
    FILE *f = fopen("coachs.txt", "r");
    if (f != NULL) {
        char ligne[100];
        printf("Chargement des coachs depuis coachs.txt\n");
        
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            if (strlen(ligne) > 0) {
                // Extraire juste le nom du coach (première partie avant |)
                char coach_nom[50];
                sscanf(ligne, "%[^|]", coach_nom);
                gtk_combo_box_append_text(GTK_COMBO_BOX(combo), coach_nom);
                printf("Coach ajouté: %s\n", coach_nom);
            }
        }
        fclose(f);
        
        if (gtk_combo_box_get_model(GTK_COMBO_BOX(combo)) != NULL) {
            gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
        }
        printf("Coachs chargés depuis fichier\n");
    } else {
        // Valeurs par défaut
        printf("Fichier coachs.txt non trouvé, chargement des coachs par défaut\n");
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "C.Omar");
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "A.Lina");
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "H.Rached");
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Z.Hamza");
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
        printf("Coachs par défaut chargés\n");
    }
}

void get_statistiques(int *total_cours, int *total_inscrits, int *cours_collectif, int *cours_individuel) {
    *total_cours = 0;
    *total_inscrits = 0;
    *cours_collectif = 0;
    *cours_individuel = 0;
    
    Cours liste[MAX_COURS];
    int nb_cours = get_tous_cours(liste);
    *total_cours = nb_cours;
    
    for (int i = 0; i < nb_cours; i++) {
        int inscrits = get_nombre_inscriptions(liste[i].id);
        *total_inscrits += inscrits;
        
        if (strcmp(liste[i].type, "collectif") == 0) {
            (*cours_collectif)++;
        } else if (strcmp(liste[i].type, "Individuel") == 0) {
            (*cours_individuel)++;
        }
    }
}

// ==================== FONCTIONS D'ÉDITION DANS TREEVIEW ====================

void on_nom_cours_edited(GtkCellRendererText *renderer, 
                         gchar *path, 
                         gchar *new_text, 
                         gpointer user_data) {
    GtkTreeView *treeview = GTK_TREE_VIEW(user_data);
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    GtkTreeIter iter;
    
    if (gtk_tree_model_get_iter_from_string(model, &iter, path)) {
        gchar *id;
        gtk_tree_model_get(model, &iter, 0, &id, -1);
        
        // Mettre à jour le cours dans le fichier
        Cours c = rechercher_cours(id);
        if (strlen(c.id) > 0) {
            printf("Modification du nom du cours %s: %s -> %s\n", id, c.nom, new_text);
            strcpy(c.nom, new_text);
            modifier_cours(c);
            
            // Mettre à jour le TreeView
            GtkListStore *store = GTK_LIST_STORE(model);
            gtk_list_store_set(store, &iter, 1, new_text, -1);
        }
        
        g_free(id);
    }
}

void on_type_cours_edited(GtkCellRendererText *renderer, 
                         gchar *path, 
                         gchar *new_text, 
                         gpointer user_data) {
    GtkTreeView *treeview = GTK_TREE_VIEW(user_data);
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    GtkTreeIter iter;
    
    if (gtk_tree_model_get_iter_from_string(model, &iter, path)) {
        gchar *id;
        gtk_tree_model_get(model, &iter, 0, &id, -1);
        
        Cours c = rechercher_cours(id);
        if (strlen(c.id) > 0) {
            printf("Modification du type du cours %s: %s -> %s\n", id, c.type, new_text);
            strcpy(c.type, new_text);
            modifier_cours(c);
            
            GtkListStore *store = GTK_LIST_STORE(model);
            gtk_list_store_set(store, &iter, 2, new_text, -1);
        }
        
        g_free(id);
    }
}

void on_niveau_cours_edited(GtkCellRendererText *renderer, 
                           gchar *path, 
                           gchar *new_text, 
                           gpointer user_data) {
    GtkTreeView *treeview = GTK_TREE_VIEW(user_data);
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    GtkTreeIter iter;
    
    if (gtk_tree_model_get_iter_from_string(model, &iter, path)) {
        gchar *id;
        gtk_tree_model_get(model, &iter, 0, &id, -1);
        
        Cours c = rechercher_cours(id);
        if (strlen(c.id) > 0) {
            printf("Modification du niveau du cours %s: %s -> %s\n", id, c.niveau, new_text);
            strcpy(c.niveau, new_text);
            modifier_cours(c);
            
            GtkListStore *store = GTK_LIST_STORE(model);
            gtk_list_store_set(store, &iter, 3, new_text, -1);
        }
        
        g_free(id);
    }
}

void on_coach_cours_edited(GtkCellRendererText *renderer, 
                          gchar *path, 
                          gchar *new_text, 
                          gpointer user_data) {
    GtkTreeView *treeview = GTK_TREE_VIEW(user_data);
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    GtkTreeIter iter;
    
    if (gtk_tree_model_get_iter_from_string(model, &iter, path)) {
        gchar *id;
        gtk_tree_model_get(model, &iter, 0, &id, -1);
        
        Cours c = rechercher_cours(id);
        if (strlen(c.id) > 0) {
            printf("Modification du coach du cours %s: %s -> %s\n", id, c.coach, new_text);
            strcpy(c.coach, new_text);
            modifier_cours(c);
            
            GtkListStore *store = GTK_LIST_STORE(model);
            gtk_list_store_set(store, &iter, 7, new_text, -1);
        }
        
        g_free(id);
    }
}

// ==================== FONCTIONS DE VALIDATION ====================

int verifier_id_unique(char *id) {
    return !cours_existe(id);
}

int verifier_format_heure(int heure, int minute) {
    return (heure >= 0 && heure < 24 && minute >= 0 && minute < 60);
}

int verifier_format_id(char *id) {
    // Vérifier que l'ID ne contient que des chiffres
    for (int i = 0; id[i] != '\0'; i++) {
        if (id[i] < '0' || id[i] > '9') {
            return 0;
        }
    }
    return 1;
}

// ==================== FONCTIONS STATISTIQUES AVANCÉES ====================

void get_statistiques_detaillees(int *total_cours, int *total_inscrits,
                                int *cours_collectif, int *cours_individuel,
                                int *inscriptions_par_jour, float *moyenne_inscrits) {
    *total_cours = 0;
    *total_inscrits = 0;
    *cours_collectif = 0;
    *cours_individuel = 0;
    *moyenne_inscrits = 0;
    
    // Initialiser le tableau des jours
    for (int i = 0; i < 5; i++) {
        inscriptions_par_jour[i] = 0;
    }
    
    Cours liste[MAX_COURS];
    int nb_cours = get_tous_cours(liste);
    *total_cours = nb_cours;
    
    for (int i = 0; i < nb_cours; i++) {
        int inscrits = get_nombre_inscriptions(liste[i].id);
        *total_inscrits += inscrits;
        
        if (strcmp(liste[i].type, "collectif") == 0) {
            (*cours_collectif)++;
        } else if (strcmp(liste[i].type, "Individuel") == 0) {
            (*cours_individuel)++;
        }
        
        // Compter les inscriptions par jour
        if (strstr(liste[i].jours, "Lundi")) inscriptions_par_jour[0] += inscrits;
        if (strstr(liste[i].jours, "Mardi")) inscriptions_par_jour[1] += inscrits;
        if (strstr(liste[i].jours, "Mercredi")) inscriptions_par_jour[2] += inscrits;
        if (strstr(liste[i].jours, "Jeudi")) inscriptions_par_jour[3] += inscrits;
        if (strstr(liste[i].jours, "Vendredi")) inscriptions_par_jour[4] += inscrits;
        
        *moyenne_inscrits += inscrits;
    }
    
    if (nb_cours > 0) {
        *moyenne_inscrits /= nb_cours;
    }
}
