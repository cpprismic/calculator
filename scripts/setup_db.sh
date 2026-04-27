#!/usr/bin/env bash
# Creates PostgreSQL user, database and grants privileges.
# Also generates config.json in the project root.
#
# Usage:
#   ./scripts/setup_db.sh [options]
#
# Options (can also be set via environment variables):
#   -h HOST       PostgreSQL host      (default: localhost)
#   -p PORT       PostgreSQL port      (default: 5432)
#   -d DBNAME     Database name        (default: calculator)
#   -u USER       Database user name   (default: calc_user)
#   -w PASSWORD   Password             (default: calc_password)
#
# Example:
#   ./scripts/setup_db.sh -d mydb -u myuser -w mysecret

set -euo pipefail

# Default values
DB_HOST="localhost"
DB_PORT="5432"
DB_NAME="calculator"
DB_USER="calc_user"
DB_PASSWORD="calc_password"

# Parse arguments
while getopts "h:p:d:u:w:" opt; do
    case "$opt" in
        h) DB_HOST="$OPTARG" ;;
        p) DB_PORT="$OPTARG" ;;
        d) DB_NAME="$OPTARG" ;;
        u) DB_USER="$OPTARG" ;;
        w) DB_PASSWORD="$OPTARG" ;;
        *) echo "Unknown option: $opt" >&2; exit 1 ;;
    esac
done

# Helper function: execute SQL as postgres user
run_as_postgres() {
    sudo -u postgres psql --no-psqlrc -v ON_ERROR_STOP=1 "$@"
}

echo "==> Setting up PostgreSQL database"
echo "    Host:     $DB_HOST:$DB_PORT"
echo "    Database: $DB_NAME"
echo "    User:     $DB_USER"
echo ""

# Create user (if not exists)
echo "--> Creating user '$DB_USER'..."
run_as_postgres <<SQL
DO \$\$
BEGIN
    IF NOT EXISTS (SELECT FROM pg_roles WHERE rolname = '$DB_USER') THEN
        CREATE USER "$DB_USER" WITH PASSWORD '$DB_PASSWORD';
        RAISE NOTICE 'User % created.', '$DB_USER';
    ELSE
        ALTER USER "$DB_USER" WITH PASSWORD '$DB_PASSWORD';
        RAISE NOTICE 'User % already exists, password updated.', '$DB_USER';
    END IF;
END
\$\$;
SQL

# Create database (if not exists)
echo "--> Creating database '$DB_NAME'..."
run_as_postgres <<SQL
SELECT 'CREATE DATABASE "$DB_NAME" OWNER "$DB_USER"'
WHERE NOT EXISTS (SELECT FROM pg_database WHERE datname = '$DB_NAME')
\gexec
SQL

# Grant privileges
echo "--> Granting privileges..."
run_as_postgres <<SQL
GRANT ALL PRIVILEGES ON DATABASE "$DB_NAME" TO "$DB_USER";
SQL

run_as_postgres -d "$DB_NAME" <<SQL
GRANT ALL ON SCHEMA public TO "$DB_USER";
SQL

# Generate config.json
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
CONFIG_FILE="$PROJECT_ROOT/config.json"

echo "--> Writing $CONFIG_FILE..."
cat > "$CONFIG_FILE" <<JSON
{
    "database": {
        "host": "$DB_HOST",
        "port": $DB_PORT,
        "dbname": "$DB_NAME",
        "user": "$DB_USER",
        "password": "$DB_PASSWORD"
    }
}
JSON

echo ""
echo "==> Done!"
echo "    config.json created: $CONFIG_FILE"
echo "    Run application: ./build/calculator '{\"operation\":\"add\",\"operands\":[2,3]}'"