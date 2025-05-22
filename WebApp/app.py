import streamlit as st
import pandas as pd
import requests

st.title("📊 Visualisation des capteurs avec analyse")

url = "http://45.140.165.126:5000/get_data"

@st.cache_data
def load_data():
    response = requests.get(url)
    response.raise_for_status()
    json_data = response.json()
    df = pd.DataFrame(json_data["data"])
    df["datetime"] = pd.to_datetime(df["date"] + " " + df["hour"])
    df = df.sort_values("datetime")
    return df

def afficher_graphique_et_analyse(df, colonne, nom_affichage, emoji):
    st.subheader(f"{emoji} {nom_affichage}")
    st.line_chart(df.set_index("datetime")[colonne])

    series = df[colonne]
    st.markdown(f"""
    **Analyse :**
    - Valeur moyenne : `{series.mean():.2f}`
    - Valeur minimale : `{series.min():.2f}`
    - Valeur maximale : `{series.max():.2f}`
    - Variation totale : `{series.max() - series.min():.2f}`
    """)

try:
    df = load_data()
    st.success("Données récupérées avec succès")

    st.subheader("📋 Toutes les données (scrollable)")
    st.dataframe(df, height=400, use_container_width=True)

    options = ["Tout", "Température", "Humidité", "Gaz", "Vapeur"]
    selected = st.multiselect("Sélectionnez les graphiques à afficher :", options, default=["Tout"])

    if "Tout" in selected:
        selected = ["Température", "Humidité", "Gaz", "Vapeur"]

    if "Température" in selected:
        afficher_graphique_et_analyse(df, "temperature", "Température", "🌡️")

    if "Humidité" in selected:
        afficher_graphique_et_analyse(df, "humidity", "Humidité", "💧")

    if "Gaz" in selected:
        afficher_graphique_et_analyse(df, "gas", "Gaz", "🟤")

    if "Vapeur" in selected:
        afficher_graphique_et_analyse(df, "steam", "Vapeur", "🌫️")

except Exception as e:
    st.error(f"Erreur : {e}")
