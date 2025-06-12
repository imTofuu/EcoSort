#!/bin/bash

npm install

echo "Waiting for db to be ready... (im in pain)"
sleep 20

echo "Migrating now. If this fails, check db logs and restart this container."
npx prisma migrate deploy

echo "Starting application"
npm run start