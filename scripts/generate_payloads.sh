#!/bin/bash
# ==============================================================================
# Script Name: generate_payloads.sh
# Description: Generates randomized payload strings, JSON templates, and XML snippets
#              for dynamic testing and attack module integration.
# ==============================================================================

OUTPUT_DIR="./assets/payloads"
mkdir -p "$OUTPUT_DIR"

echo "[+] Generating randomized payloads..."

# 1. Generate Random String Payloads
echo "[*] Creating random alphanumeric strings..."
for i in {1..100}; do
    tr -dc 'A-Za-z0-9-_' </dev/urandom | head -c $((RANDOM % 256 + 32)) >> "$OUTPUT_DIR/random_strings.txt"
    echo "" >> "$OUTPUT_DIR/random_strings.txt"
done

# 2. Generate Dynamic JSON Templates
echo "[*] Creating dynamic JSON payload templates..."
cat << 'EOF' > "$OUTPUT_DIR/templates.json"
[
  {
    "type": "auth_brute",
    "template": "{\"username\": \"user_RANDOM_ID\", \"password\": \"pass_RANDOM_HASH\"}"
  },
  {
    "type": "search_hammer",
    "template": "{\"query\": \"RANDOM_TERM\", \"limit\": 100, \"offset\": 0}"
  },
  {
    "type": "api_payload",
    "template": "{\"transaction_id\": \"UUID_GEN\", \"amount\": RANDOM_INT, \"timestamp\": TIMESTAMP}"
  }
]
EOF

# 3. Generate XML Snippets for SOAP/XML Hammering
echo "[*] Creating XML payloads..."
cat << 'EOF' > "$OUTPUT_DIR/templates.xml"
<root>
    <request id="RANDOM_ID">
        <user>admin_RANDOM_USER</user>
        <action>execute_query</action>
        <payload>RANDOM_CONTENT</payload>
    </request>
</root>
EOF

echo "[+] Payloads successfully generated in $OUTPUT_DIR/"
